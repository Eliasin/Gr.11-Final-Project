#include <string>
#include <iostream>
#include <vector>
#include "gameLogic.hpp"
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#undef main

int main(int argc, char * argv[]) {
    Game::Map map;

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Summative");
    unsigned int currentColour = 0;
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
