#include <SFML/Graphics.hpp>
#include "starter.h"

///using namespace sf;

Texture sprTexture;
Texture backTexture;
Starter* starter;

int main() {
	
    while (!sprTexture.loadFromFile("texture/PMSprites.png")) {};

    // R�cup�rer la r�solution de l'�cran
    VideoMode desktopMode = VideoMode::getDesktopMode();
    int winW = desktopMode.width;
    int winH = desktopMode.height * 0.9; // Ajustement pour garder une petite marge en bas

    RenderWindow window(VideoMode(winW, winH), "ii", Style::Default);
    window.setActive(false);

    starter = new Starter(&window, &sprTexture);
	
	
    std::cout << "Closed window " << std::endl;

    delete starter;
    return 0;
}
