#include "game.hpp"

namespace Main {

    GameInstance::GameInstance() {
        frameClock = sf::Clock();
        map = Game::Map();
        entityTemplates = std::map<std::string, Game::EntityTemplate>();
        sf::RenderWindow window;
        textureSets = std::map<std::string, std::map<std::string, sf::Texture>>();
        entityRenderers = std::vector<Rendering::EntityRenderer>();
        backgrounds = std::vector<Rendering::Background>();
        backgroundTextures = std::map<std::string, sf::Texture>();
        camera = Rendering::Camera();
        keyHandlers = std::vector<IO::KeyHandler*>();
        exitGame = false;
        absoluteBackgroundTexture = sf::Texture();
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

    void GameInstance::initializeWindow() {
        window.create(sf::VideoMode::getDesktopMode(), "Gr.11-Final-Project");
        camera.setPos(Game::Vector(0, 0));
    }

    void GameInstance::loadTextureSetFromPath(std::string setPath, std::string name) {
        std::map<std::string, sf::Texture> textureSet;
        for (std::string currentState : Rendering::EntityRenderer::stateTextureNames) {
            sf::Texture texture;
            std::cout << "Attempting to load " << setPath + "/" + currentState + ".png" << std::endl;
            if (!texture.loadFromFile(setPath + "/"  + currentState + ".png")) {
                texture.loadFromFile(setPath + "/idle.png");
            }
            else {
                std::cout << "Loaded " << setPath + "/" + currentState + ".png" << " successfully." << std::endl;
            }

            textureSet[currentState] = texture;
        }
        textureSets[name] = textureSet;
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
        if (font.loadFromFile("arial.ttf")) {
            fonts["name"] = font;
        }
    }

    void GameInstance::initializeTextures() {
        loadTextureSetFromPath("resources/textures/player", "player");
        loadBackgroundTextureFromPath("resources/textures/brick.png", "brick");
        absoluteBackgroundTexture.loadFromFile("resources/textures/background.png");
        loadFontFromPath("resources/fonts/arial.ttf", "arial");
    }

    void GameInstance::initializeIO() {
        std::map<sf::Keyboard::Key, Game::Vector> wasdMovementMap = {{sf::Keyboard::W, Game::Vector(0, -2)}, {sf::Keyboard::A, Game::Vector(-2, 0)}, {sf::Keyboard::S, Game::Vector(0, 2)}, {sf::Keyboard::D, Game::Vector(2, 0)}};
        keyHandlers.push_back(new IO::EntityMovementKeyHandler(wasdMovementMap, &map, 0));
    }

    void GameInstance::initializeGameLogic() {
        Game::EntityStats defaultStats;
        Game::Rect defaultHitbox(Game::Vector(0, 0), 100, 100);
        entityTemplates["player"] = Game::EntityTemplate(defaultStats, defaultHitbox, NULL);
        map.createEntity(entityTemplates["player"]);
        map.setPlayableArea(Game::Rect(Game::Vector(-2000, -2000), 4000, 4000));
    }

    void GameInstance::initializeRendering() {
        camera.setViewBox(Game::Rect(Game::Vector(0, 0), 1920, 1080));
        Rendering::EntityRenderer playerRenderer = Rendering::EntityRenderer(Rendering::EntityEventParser(&map, 0), &camera, &window, &textureSets["player"]);
        entityRenderers.push_back(playerRenderer);
        backgrounds.push_back(Rendering::Background(&backgroundTextures["brick"], &camera, &window, Game::Rect(Game::Vector(-2000, -2000), 4000, 4000)));

        absoluteBackgroundSprite.setTexture(absoluteBackgroundTexture);
        absoluteBackgroundSprite.scale(Rendering::scaleSpriteRelativeToWindow(absoluteBackgroundSprite, window, sf::Vector2<float>(1.f, 1.f)));

        for (unsigned int i = 0; i < 5; i++) {
            lastFrameTimes.push_back(0.f);
        }

        fpsText.setFont(fonts["arial"]);
        fpsText.setCharacterSize(32);
        fpsText.setColor(sf:Color:Yellow);
    }

    void GameInstance::initializeGame() {
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
    }

    void GameInstance::cullRenderers() {
        for (std::vector<Rendering::EntityRenderer>::iterator currentRenderer = entityRenderers.begin(); currentRenderer != entityRenderers.end(); currentRenderer++) {
            if (!currentRenderer->getEntityEventParser().entityValid()) {
                currentRenderer = entityRenderers.erase(currentRenderer);
            }
        }
    }

    void GameInstance::tickRendering() {
        cullRenderers();
        window.clear(sf::Color::White);
        window.draw(absoluteBackgroundSprite);
        camera.centerOn(map.getEntityWithID(0)->getHitbox().getCenter(), window);
        for (Rendering::Background background : backgrounds) {
            background.updateBackgroundSprite();
            window.draw(background.getSprite());
        }
        for (Rendering::EntityRenderer& currentRenderer : entityRenderers) {
            currentRenderer.updateEntitySprite();
            window.draw(currentRenderer.getSprite());
        }
        window.draw(fpsText);
        window.display();
    }

    void GameInstance::tickGame() {

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
