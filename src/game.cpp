#include "game.hpp"

namespace Main {

    GameInstance::GameInstance() {
        frameClock = sf::Clock();
        map = Game::Map();
        entityTemplates = std::map<std::string, Game::EntityTemplate>();
        sf::RenderWindow window;
        textureSets = std::map<std::string, std::map<std::string, std::vector<sf::Texture>>>();
        entityRenderers = std::vector<Rendering::EntityRenderer>();
        backgrounds = std::vector<Rendering::Background>();
        backgroundTextures = std::map<std::string, sf::Texture>();
        camera = Rendering::Camera();
        keyHandlers = std::vector<IO::KeyHandler*>();
        mouseHandlers = std::vector<IO::MouseHandler*>();
        exitGame = false;
        absoluteBackgroundTextures = std::map<std::string, std::vector<sf::Texture>>();
        videoModes = std::vector<sf::VideoMode>();
    }

    void GameInstance::addFrameTimeToAvg(float frameTime) {
        lastFrameTimes.push_back(frameTime);
        lastFrameTimes.pop_front();
    }

    float GameInstance::getAvgFPS() {
        float frameTimeSum = 0.f;
        for (float frameTime : lastFrameTimes) {
            frameTimeSum += frameTime;
        }
        float averageFrameTime = frameTimeSum / (float)lastFrameTimes.size();
        return pow(averageFrameTime, -1);
    }

    void GameInstance::initializeVideoModes() {
        videoModes.push_back(sf::VideoMode(1920, 1080));
        videoModes.push_back(sf::VideoMode(1600, 900));
        videoModes.push_back(sf::VideoMode(1366, 768));
        videoModes.push_back(sf::VideoMode(1280, 720));
        videoModes.push_back(sf::VideoMode(1152, 648));
        videoModes.push_back(sf::VideoMode(1024, 576));
    }

    sf::VideoMode GameInstance::getLargestCompatibleResolution() {
        sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
        for (sf::VideoMode videoMode : videoModes) {
            if (videoMode.width <= desktopMode.width && videoMode.height <= desktopMode.height) {
                return videoMode;
            }
        }
        return desktopMode;
    }

    void GameInstance::initializeWindow() {
        window.create(getLargestCompatibleResolution(), "Gr.11-Final-Project", sf::Style::None | sf::Style::Titlebar | sf::Style::Close);
        camera.setPos(Game::Vector(0, 0));
    }

    void GameInstance::loadTextureSetFromPath(std::string setPath, std::string name) {
        std::map<std::string, std::vector<sf::Texture>> textureSet;
        for (std::string currentState : Rendering::EntityRenderer::stateTextureNames) {
            std::cout << "Attempting to load " << name + " " + currentState + " texture set." << std::endl;
            std::vector<sf::Texture> frameSet;
            for (unsigned int i = 0;; i++) {
                std::string fileName = setPath + "/"  + currentState + "/" + std::to_string(i) + ".png";
                std::ifstream file(fileName);
                if (file) {
                    sf::Texture texture;
                    texture.loadFromFile(fileName);
                    frameSet.push_back(texture);
                    std::cout << "Loaded " << fileName << " successfully." << std::endl;
                }
                else {
                    break;
                }
            }
            textureSet[currentState] = frameSet;
        }
        textureSets[name] = textureSet;
    }

    void GameInstance::loadAbsoluteBackgroundTexturesFromPath(std::string path, std::string name) {
        std::cout << "Attempting to load " + name + " absolute background texture set." << std::endl;
        std::vector<sf::Texture> textureSet;
        for (unsigned int i = 0;; i++) {
            std::string filePath = path + "/" + std::to_string(i) + ".png";
            std::ifstream file(filePath);
            if (file) {
                sf::Texture texture;
                texture.loadFromFile(filePath);
                textureSet.push_back(texture);
                std::cout << "Loaded " << filePath << " successfully." << std::endl;
            }
            else {
                break;
            }
        }
        absoluteBackgroundTextures[name] = textureSet;
    }

    void GameInstance::loadBackgroundTextureFromPath(std::string path, std::string name) {
        sf::Texture texture;
        if (texture.loadFromFile(path)) {
            backgroundTextures[name] = texture;
            backgroundTextures[name].setRepeated(true);
        }
    }

    void GameInstance::loadFontFromPath(std::string path, std::string name) {
        sf::Font font;
        if (font.loadFromFile(path)) {
            fonts[name] = font;
        }
    }

    void GameInstance::initializeTextures() {
        loadTextureSetFromPath("resources/textures/player", "player");
        loadTextureSetFromPath("resources/textures/dummy", "dummy");
        loadBackgroundTextureFromPath("resources/textures/brick.png", "brick");
        loadFontFromPath("resources/fonts/arial.ttf", "arial");
        loadAbsoluteBackgroundTexturesFromPath("resources/textures/starry", "starry");
    }

    void GameInstance::initializeIO() {
        std::map<sf::Keyboard::Key, Game::Vector> wasdMovementMap = {{sf::Keyboard::W, Game::Vector(0, -1)}, {sf::Keyboard::A, Game::Vector(-1, 0)}, {sf::Keyboard::S, Game::Vector(0, 1)}, {sf::Keyboard::D, Game::Vector(1, 0)}};
        keyHandlers.push_back(new IO::EntityMovementKeyHandler(wasdMovementMap, &map, 0));

        mouseHandlers.push_back(new IO::PlayerAttackMouseHandler(0, &map, &camera));
    }

    void GameInstance::initializeGameLogic() {
        Game::EntityStats defaultStats;
        Game::Rect defaultHitbox(Game::Vector(0, 0), 100, 100);

        Game::Rect testDummyHitbox(Game::Vector(100, 100), 100, 100);

        entityTemplates["player"] = Game::EntityTemplate(defaultStats, defaultHitbox, NULL, Game::Team::TEAM::PLAYER);
        entityTemplates["testDummy"] = Game::EntityTemplate(defaultStats, testDummyHitbox, NULL, Game::Team::TEAM::ENEMY);

        map.setPlayableArea(Game::Rect(Game::Vector(-2000, -2000), 4000, 4000));
        map.createEntity(entityTemplates["player"]);
        map.createEntity(entityTemplates["testDummy"]);
    }

    void GameInstance::initializeRendering() {
        camera.setViewBox(Game::Rect(Game::Vector(0, 0), window.getSize().x, window.getSize().y));
        Rendering::EntityRenderer playerRenderer = Rendering::EntityRenderer(Rendering::EntityEventParser(&map, 0), &camera, &window, &textureSets["player"], 30);
        entityRenderers.push_back(playerRenderer);
        backgrounds.push_back(Rendering::Background(&backgroundTextures["brick"], &camera, &window, Game::Rect(Game::Vector(-2000, -2000), 4000, 4000)));
        for (unsigned int i = 0; i < 210; i++) {
            lastFrameTimes.push_back(TIME_PER_FRAME);
        }

        fpsText.setFont(fonts["arial"]);
        fpsText.setCharacterSize(32);
        fpsText.setFillColor(sf::Color::Yellow);
        fpsText.setOutlineColor(sf::Color::Black);
        fpsText.setPosition(sf::Vector2<float>(0, 0));
        fpsText.setStyle(sf::Text::Bold);

        absoluteBackground = Rendering::AbsoluteBackground(&absoluteBackgroundTextures["starry"], &window, 10);
        absoluteBackground.setLooping(true);

        entityRenderers.push_back(Rendering::EntityRenderer(Rendering::EntityEventParser(&map, 1), &camera, &window, &textureSets["dummy"], 30));

    }

    void GameInstance::initializeGame() {
        initializeVideoModes();
        initializeWindow();
        initializeTextures();
        initializeIO();
        initializeGameLogic();
        initializeRendering();
    }

    void GameInstance::tickIO() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                exitGame = true;
            }
        }

        for (IO::KeyHandler* currentKeyHandler : keyHandlers) {
            currentKeyHandler->checkForKeyPress();
        }

        for (IO::MouseHandler* currentMouseHandler : mouseHandlers) {
            currentMouseHandler->checkForMouseEvents();
        }
    }

    void GameInstance::cullRenderers() {
        std::vector<std::vector<Rendering::EntityRenderer>::iterator> needsErasing;
        for (std::vector<Rendering::EntityRenderer>::iterator currentRenderer = entityRenderers.begin(); currentRenderer != entityRenderers.end(); currentRenderer++) {
            if (!currentRenderer->getEntityEventParser().entityValid()) {
                needsErasing.push_back(currentRenderer);
            }
        }
        for (std::vector<Rendering::EntityRenderer>::iterator erasing : needsErasing) {
            entityRenderers.erase(erasing);
        }
    }

    void GameInstance::drawBackgrounds() {
        for (Rendering::Background background : backgrounds) {
            background.updateBackgroundSprite();
            window.draw(background.getSprite());
        }
    }

    void GameInstance::drawEntities() {
        for (Rendering::EntityRenderer& currentRenderer : entityRenderers) {
            currentRenderer.updateEntitySprite();
            window.draw(currentRenderer.getSprite());
        }
    }

    void GameInstance::updateFPSText() {
        fpsText.setString(std::to_string(static_cast<int>(std::ceil(getAvgFPS()))));
    }

    void GameInstance::tickRendering() {
        cullRenderers();
        window.clear(sf::Color::White);

        camera.centerOn(map.getEntityWithID(0)->getHitbox().getCenter(), window);

        absoluteBackground.tick();
        window.draw(absoluteBackground.getSprite());

        drawBackgrounds();
        drawEntities();
        updateFPSText();
        window.draw(fpsText);

        window.display();
    }

    void GameInstance::tickGame() {
        map.tickAndApplyActions();
    }

    void GameInstance::run() {
        initializeGame();
        while (!exitGame) {
            frameClock.restart();
            tickIO();
            tickGame();
            tickRendering();

            if (frameClock.getElapsedTime().asSeconds() < TIME_PER_FRAME) {
                sf::sleep(sf::seconds(TIME_PER_FRAME) - frameClock.getElapsedTime());
            }
            addFrameTimeToAvg(frameClock.getElapsedTime().asSeconds());
        }
    }

}
