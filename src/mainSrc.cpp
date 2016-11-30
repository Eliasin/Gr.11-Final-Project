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
    mainCamera.setPos(Game::Vector(0,0));

    Game::Vector playerPos(700, 600);

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("resources/textures/player.png")) {

    }

    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    playerSprite.setScale(0.25f, 0.25f);

    sf::Keyboard keyboard;
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Summative");

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (keyboard.isKeyPressed(sf::Keyboard::W)) {
            mainCamera.move(Game::Vector(0, -1));
            playerPos.y -= 1;
        }
        else if (keyboard.isKeyPressed(sf::Keyboard::S)) {
            mainCamera.move(Game::Vector(0, 1));
            playerPos.y += 1;
        }
        else if (keyboard.isKeyPressed(sf::Keyboard::A)) {
            mainCamera.move(Game::Vector(-1, 0));
            playerPos.x -= 1;
        }
        else if (keyboard.isKeyPressed(sf::Keyboard::D)) {
            mainCamera.move(Game::Vector(1, 0));
            playerPos.x += 1;
        }

        playerSprite.setPosition(mainCamera.translate(playerPos));

        window.clear(sf::Color::White);
        window.draw(playerSprite);
        window.display();
    }

    return 0;
}
