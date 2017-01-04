#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "gameLogic.hpp"
#include "rendering.hpp"
#include "io.hpp"

namespace Main {

    struct OverflowData {
        std::vector<Rendering::Background> backgrounds;
        std::map<std::string, sf::Texture> backgroundTextures;
    };

    class GameInstance {
        Game::Map map;
        std::map<std::string, Game::EntityTemplate> entityTemplates;
        sf::RenderWindow window;
        std::map<std::string, std::map<std::string, sf::Texture>> textureSets;
        std::vector<Rendering::EntityRenderer> entityRenderers;
        Rendering::Camera camera;
        std::vector<IO::KeyHandler*> keyHandlers;

        bool exitGame;

        std::vector<unsigned int> getEntitiesWithoutRenderer();

        void loadTextureSetFromPath(std::string setPath, std::string name);
        void loadBackgroundTextureFromPath(std::string, std::string name);

        void initializeWindow();
        void initializeTextures();
        void initializeIO();
        void initializeGameLogic();
        void initializeRendering();
        void initializeGame();

        void tickIO();
        void tickGameLogic();

        void cullRenderers();
        void tickRendering();
        void tickGame();
    public:
        OverflowData* data;
        GameInstance();
        void run();
    };

}
