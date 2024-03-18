#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
#include <SFML/Graphics.hpp>
#include "pac.h"
#include "ghost.h"
#include "maze.h"
#include "gameText.h"
#include "fruit.h"

sf::Font font; // Déclaration de la police de caractères pour le texte
sf::Text pauseText; // Déclaration du texte pour la pause
sf::Text resumeText; // Déclaration du texte pour la reprise

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Starter
{

public:
    bool isPaused = false; // Déclaration de la variable isPaused

    int i = 0;

    sf::Event sfEvt;
    Maze maze;
    Pac<Starter>* pac;
    Ghost<Starter>* Blinky, * Pinky, * Inky, * Clyde;
    std::thread ghStatusThread;
    std::thread loopThread;
    int attackInterval = 15; // 15 sec
    int scaterInterval = 7; // 7 sec
    int blueInterval = 6;
    int delay;
    int curTime;
    sf::Texture backText;
    sf::Texture backFlashText;

    sf::Sprite backSpr;
    bool intro = false;
    bool isCollid = false;
    bool lifeWin = false;
    bool toNextLevel = false;
    GameSound* gmSound;
    GameText* gameText;
    Fruit* fruit;
    sf::RenderWindow* window;


    Starter(sf::RenderWindow* win, sf::Texture* sprTexture)
    {
        if (!font.loadFromFile("arial.ttf")) {
            throw std::runtime_error("Failed to load font");
        }

        pauseText.setFont(font);
        pauseText.setString("PAUSE");
        pauseText.setCharacterSize(48);
        pauseText.setFillColor(sf::Color::White);
        pauseText.setPosition(win->getSize().x / 2 - pauseText.getLocalBounds().width / 2,
            win->getSize().y / 2 - pauseText.getLocalBounds().height / 2);

        resumeText.setFont(font);
        resumeText.setString("Cliquer sur espace pour reprendre le jeu");
        resumeText.setCharacterSize(24);
        resumeText.setFillColor(sf::Color::White);
        resumeText.setPosition(win->getSize().x / 2 - resumeText.getLocalBounds().width / 2,
            pauseText.getPosition().y + pauseText.getLocalBounds().height + 20);

        window = win;
        gmSound = GameSound::getInstance();
        gameStatus = Demo;
        backText.loadFromFile("texture/map3.png");
        backFlashText.loadFromFile("texture/mapB.png");
        backSpr.setTexture(backText);
        maze.initMaze();
        pac = new Pac<Starter>(sprTexture, this);
        Blinky = new Ghost<Starter>(sprTexture, 1, this);
        Pinky = new Ghost<Starter>(sprTexture, 2, this);
        Inky = new Ghost<Starter>(sprTexture, 3, this);
        Clyde = new Ghost<Starter>(sprTexture, 4, this);
        gameText = new GameText();
        fruit = new Fruit(sprTexture);

        loopThread = std::thread([this, win]() { loop(win); });
        while (win->isOpen())
        {

            if (CntrGame::dotsEat == maze.dotsCount) { nextLevel(); }
            while (win->pollEvent(sfEvt))
            {
                if (sfEvt.type == sf::Event::Closed) { win->close(); }
                else if (sfEvt.type == sf::Event::KeyPressed)
                {
                    if (sfEvt.key.code == sf::Keyboard::Space) {
                        if (i == 0) {
                            // Mettre en pause le jeu
                            i++;
                            pac->pause();
                            Blinky->pause();
                            Pinky->pause();
                            Inky->pause();
                            Clyde->pause();
                            //gameStatus = Pause;
                        }
                        else if (i != 0) {
                            // Reprendre le jeu
                            i = 0;
                            pac->resume();
                            Blinky->resume();
                            Pinky->resume();
                            Inky->resume();
                            Clyde->resume();
                            //gameStatus = Play;
                        }
                    }
                    else if (gameStatus == Demo && sfEvt.key.code == sf::Keyboard::Enter)
                    {
                        startGame();
                    }
                    pac->rotation(sfEvt.key.code);
                }
            }
        }
    };

    ~Starter()
    {
        if (loopThread.joinable()) { loopThread.detach(); }
        if (ghStatusThread.joinable()) { ghStatusThread.detach(); }
        delete  Blinky;
        delete Pinky;
        delete Inky;
        delete Clyde;
    }

    void drawLife(sf::RenderWindow* win)
    {
        if (pac->pacLife < 0) { return; }
        sf::Sprite spr;
        for (int i = 0; i < pac->pacLife; i++)
        {
            spr = pac->getLifeSpr();
            spr.setPosition(sf::Vector2f(30 * i, 24 * 30));
            (*win).draw(spr);
        }
    }
    
    void loop(sf::RenderWindow* win)
    {
        win->setActive(true);
        while (win->isOpen())
        {
            if (CntrGame::score >= 10000 && !lifeWin)
            {
                lifeWin = true;
                gmSound->play(GameSound::PlSound::WinnLife);
                pac->pacLife++;
            }
            win->clear();
            if (gameStatus == Play)
            {
                drawLife(win);
                maze.drawWall(win);
                win->draw(backSpr);
                win->draw(gameText->gameOverTxt);
                win->draw(gameText->scoreTxt);
                win->draw(gameText->levelTxt);
                if (ghostStatus == Blue)
                {
                    std::string dif = std::to_string(curTime - time(0));
                    if (std::stoi(dif) == 0) { dif = ""; }
                    gameText->countTxt.setString(dif);
                    win->draw(gameText->countTxt);
                }
                if (gameText->bonusTxt.getString() != "")
                {
                    win->draw(gameText->bonusTxt);
                }
                if (fruit->getVisible()) { win->draw(fruit->getSrpite()); }
            }
            else
            {
                win->draw(gameText->enterTxt);
                win->draw(Blinky->getNameTxt());
                win->draw(Pinky->getNameTxt());
                win->draw(Inky->getNameTxt());
                win->draw(Clyde->getNameTxt());
            }

            win->draw(pac->getSprite());
            win->draw(Blinky->getSprite());
            win->draw(Pinky->getSprite());
            win->draw(Inky->getSprite());
            win->draw(Clyde->getSprite());
            win->display();

        }
    }

    void togglePause() {
        isPaused = !isPaused;
    }

    void drawPauseText(sf::RenderWindow* win) {
        if (isPaused) {
            // Dessiner le texte de pause et de reprise
            win->draw(pauseText);
            win->draw(resumeText);
        }
    }

    void changeGhostState()
    {
        delay = scaterInterval;
        if (ghostStatus == Blue)
        {
            delay = blueInterval;
            CntrGame::isBlueGhost = true;
        }
        changeStatus();
        while (ghStatusThread.joinable())
        {
            curTime = time(0);
            curTime += delay;

            while (true && ghStatusThread.joinable())
            {
                if (curTime <= time(0)) { break; }
            }// wait for change ghost status

            sleep(milliseconds(10));
            if (ghostStatus == Blue) // end blue time
            {
                CntrGame::isBlueGhost = false;
                CntrGame::ghostBonus = 100;
                gmSound->stop(GameSound::PlSound::Blue);
                if (CntrGame::gameRun)gmSound->play(GameSound::PlSound::Siren);
            }
            if (ghostStatus == Attack)
            {
                ghostStatus = Scater;
                delay = scaterInterval;
            }
            else
            {
                ghostStatus = Attack;
                delay = attackInterval;
            }
            changeStatus();

            if ((maze.dotsCount - CntrGame::dotsEat) < 30)
            {
                gmSound->setPich(GameSound::Siren, 1.2);
            }
        }
    }

    void startGame()
    {
        maze.redrawDot();
        gameText->gameOverTxt.setString("");
        pac->pacLife = 2;
        gameStatus = Play;
        resetPacGhost();
        intro = true;
        CntrGame::gameRun = true;
        CntrGame::level = 1;
        CntrGame::score = 0;
        CntrGame::dotsEat = 0;
        blueInterval = 6;
        fruit->setLevel(CntrGame::level);
        gmSound->setPich(GameSound::Siren, 1);
        startLevel();
    }

    void startLevel()
    {
        if (intro)
        {
            intro = false;
            gmSound->play(GameSound::PlSound::Intro);
        }
        isCollid = false;
        ghostStatus = Scater;
        gmSound->play(GameSound::PlSound::Siren);
        CntrGame::gameRun = true;
        resetPacGhost();
        pac->run();
        creatGhostThr();
        fruit->start();
        gameText->levelTxt.setString("LEVEL: " + std::to_string(CntrGame::level));
    }

    void nextLevel()
    {
        gmSound->setPich(GameSound::Siren, 1);
        toNextLevel = true;
        CntrGame::dotsEat = 0;
        stopAll();
        maze.redrawDot();
        CntrGame::level++;
        gameText->levelTxt.setString("LEVEL: " + std::to_string(CntrGame::level));
        fruit->setLevel(CntrGame::level);
        if (CntrGame::level > 2 && CntrGame::level < 6) { blueInterval = 4; }
        else if (CntrGame::level >= 6 && CntrGame::level < 10) { blueInterval = 2; }
        else if (CntrGame::level >= 10) { blueInterval = 0; }
    }

    void creatGhostThr()
    {
        while (ghStatusThread.joinable()) {}
        ghStatusThread = std::thread(&Starter::changeGhostState, this);
    }

    void resetLevel()
    {
        CntrGame::ghostBonus = 100;
        if (pac->pacLife < 0)
        {
            gameText->gameOverTxt.setString("GAME OVER");
        }
    }
    void stopAll() {
        pac->stop();
        Blinky->stop();
        Pinky->stop();
        Inky->stop();
        Clyde->stop();
        // Ajoutez d'autres actions à arrêter si nécessaire
    }
    void resetPacGhost()
    {
        pac->reset();
        Blinky->reset();
        Pinky->reset();
        Inky->reset();
        Clyde->reset();
    }
    void changeStatus()
    {
        Blinky->changeGhostState();
        Pinky->changeGhostState();
        Inky->changeGhostState();
        Clyde->changeGhostState();
    }


    void wait(int delayInt)
    {
        auto curTime = time(0);
        int counter = 0;
        curTime += delayInt;
        while (true)
        {
            counter++;
            if (toNextLevel)
            {
                if (counter % 30 == 0)
                {
                    backSpr.setTexture(backFlashText);
                }
                else if (counter % 30 == 15)
                {
                    backSpr.setTexture(backText);
                }
            }
            if (curTime < time(0)) { break; }
            sleep(milliseconds(10));
        }
        toNextLevel = false;
        backSpr.setTexture(backText);
    }
};
