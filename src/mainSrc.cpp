#include <string>
#include <iostream>
#include <vector>
#include "gameLogic.hpp"
#include "rendering.hpp"
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main(int argc, char * argv[]) {
    Game::Map map;

    Rendering::Camera mainCamera;
    mainCamera.setPos(0,0);

    sf::Keyboard keyboard;
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Summative");

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);
        window.display();
    }

    return 0;
}
