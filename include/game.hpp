#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "gameLogic.hpp"
#include "rendering.hpp"
#include "io.hpp"
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace Main {

    class Game {
        Game::Map map;
        sf::RenderWindow window;
        std::vector<sf::Texture> textures;
        std::vector<sf::Sprites> sprites;
        Rendering::Camera camera;
        sf::Keyboard keyboard;
        std::vector<KeyHandler*> keyHandlers;
    public:
        void run();
    };

}
