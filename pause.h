// pause.h
#pragma once

#include <SFML/Graphics.hpp>

class Pause {
public:
    Pause(sf::RenderWindow& window);

    void handleEvents(sf::Event& event);
    void update();
    void draw();

    bool isPaused() const { return paused; }
    void togglePause() { paused = !paused; }

private:
    sf::RenderWindow& window;
    bool paused;
    sf::Font font;
    sf::Text pauseText;
    sf::Text resumeText;
};
