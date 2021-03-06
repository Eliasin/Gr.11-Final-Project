#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <list>
#include <fstream>
#include "gameLogic.hpp"
#include "rendering.hpp"
#include "io.hpp"

namespace Main {

    class GameInstance {
        const unsigned int FPS_CAP = 60;
        const float TIME_PER_FRAME = 1.f / static_cast<float>(FPS_CAP);
        sf::Clock frameClock;
        std::list<float> lastFrameTimes;
        sf::Text fpsText;

        std::map<std::string, sf::Font> fonts;

        Game::Map map;
        std::map<std::string, Game::EntityTemplate> entityTemplates;
        sf::RenderWindow window;
        std::map<std::string, std::map<std::string, std::vector<sf::Texture>>> textureSets;
        std::map<std::string, std::vector<sf::Texture>> attackAnimations;
        std::vector<Rendering::EntityRenderer> entityRenderers;
        std::vector<Rendering::Animation> animations;
        std::vector<Rendering::Background> backgrounds;
        std::map<std::string, sf::Texture> backgroundTextures;
        Rendering::Camera camera;
        std::vector<IO::KeyHandler*> keyHandlers;
        std::vector<IO::MouseHandler*> mouseHandlers;
        std::map<std::string, std::vector<sf::Texture>> absoluteBackgroundTextures;
        Rendering::AbsoluteBackground absoluteBackground;
        std::vector<sf::VideoMode> videoModes;

        bool exitGame;

        void addFrameTimeToAvg(float frameTime);
        float getAvgFPS();
        sf::VideoMode getLargestCompatibleResolution();

        void loadTextureSetFromPath(std::string setPath, std::string name);
        void loadAbsoluteBackgroundTexturesFromPath(std::string path, std::string name);
        void loadBackgroundTextureFromPath(std::string path, std::string name);
        void loadFontFromPath(std::string path, std::string name);
        void loadAnimationSet(std::string path, std::string name);

        void initializeWindow();
        void initializeTextures();
        void initializeVideoModes();
        void initializeIO();
        void initializeGameLogic();
        void initializeRendering();
        void initializeGame();

        void tickIO();
        void tickGameLogic();

        void cullRenderers();
        void cullAnimations();
        void drawBackgrounds();
        void drawEntities();
        void drawAnimations();
        void updateFPSText();
        void tickRendering();

        void tickGame();
    public:
        GameInstance();
        void run();
    };

}
