// pause.cpp
#include "pause.h"

Pause::Pause(sf::RenderWindow& window) : window(window), paused(false) {
    if (!font.loadFromFile("arial.ttf")) {
        // Gérer l'erreur si la police de caractères n'a pas pu être chargée
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
        togglePause(); // Bascule l'état de la pause lorsque la touche espace est pressée
    }
}

void Pause::update() {
    // Ajoutez ici le code pour mettre à jour la pause si nécessaire
}

void Pause::draw() {
    if (paused) {
        window.draw(pauseText);
        window.draw(resumeText);
    }
}
