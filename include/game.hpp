#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "gameLogic.hpp"
#include "rendering.hpp"
#include "io.hpp"

namespace Main {

    class GameInstance {
        Game::Map map;
        sf::RenderWindow window;
        std::map<std::string, std::map<std::string, sf::Texture>> textureSets;
        std::map<std::string, Rendering::RenderProfile> renderProfiles;
        Rendering::Camera camera;
        std::vector<IO::KeyHandler*> keyHandlers;

        void loadTextureSetFromPath(std::string setPath, std::string name);

        void initializeRenderingAndWindow();
        void initializeTextures();
        void initializeIO();
        void initializeGameLogic();

        void initializeGame();
        void tickGame();
    public:
        void run();
    };

}
