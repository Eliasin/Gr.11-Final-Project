#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <queue>
#include "gameLogic.hpp"
#include "rendering.hpp"
#include "io.hpp"

namespace Main {

    class GameInstance {
        const unsigned int FPS_CAP = 30;
        const float TIME_PER_FRAME = 1.f / (float)FPS_CAP;
        sf::Clock frameClock;
        std::queue<float> lastFrameTimes;

        Game::Map map;
        std::map<std::string, Game::EntityTemplate> entityTemplates;
        sf::RenderWindow window;
        std::map<std::string, std::map<std::string, sf::Texture>> textureSets;
        std::vector<Rendering::EntityRenderer> entityRenderers;
        std::vector<Rendering::Background> backgrounds;
        std::map<std::string, sf::Texture> backgroundTextures;
        Rendering::Camera camera;
        std::vector<IO::KeyHandler*> keyHandlers;
        sf::Texture absoluteBackgroundTexture;
        sf::Sprite absoluteBackgroundSprite;

        bool exitGame;

        float getAvgFPS();

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
        GameInstance();
        void run();
    };

}
