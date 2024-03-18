// pause.cpp
#include "pause.h"

Pause::Pause(sf::RenderWindow& window) : window(window), paused(false) {
    if (!font.loadFromFile("arial.ttf")) {
        // G�rer l'erreur si la police de caract�res n'a pas pu �tre charg�e
        throw std::runtime_error("Failed to load font");
    }

     pauseText.setFont(font);
    pauseText.setString("Pause");
    pauseText.setCharacterSize(48);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setPosition(window.getSize().x / 2 - pauseText.getLocalBounds().width / 2,
        window.getSize().y / 2 - pauseText.getLocalBounds().height / 2);

    resumeText.setFont(font);
    resumeText.setString("Cliquer sur espace pour reprendre le jeu");
    resumeText.setCharacterSize(24);
    resumeText.setFillColor(sf::Color::White);
    resumeText.setPosition(window.getSize().x / 2 - resumeText.getLocalBounds().width / 2,
        pauseText.getPosition().y + pauseText.getLocalBounds().height + 20);
}

void Pause::handleEvents(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
        togglePause(); // Bascule l'�tat de la pause lorsque la touche espace est press�e
    }
}

void Pause::update() {
    // Ajoutez ici le code pour mettre � jour la pause si n�cessaire
}

void Pause::draw() {
    if (paused) {
        window.draw(pauseText);
        window.draw(resumeText);
    }
}
