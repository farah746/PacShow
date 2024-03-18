#include <SFML/Graphics.hpp>
#include "starter.h"

///using namespace sf;

Texture sprTexture;
Texture backTexture;
Starter* starter;

int main() {
	
    while (!sprTexture.loadFromFile("texture/PMSprites.png")) {};

    // Récupérer la résolution de l'écran
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
