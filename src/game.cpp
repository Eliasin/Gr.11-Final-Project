#include "game.hpp"

namespace Main {

    GameInstance::GameInstance() {
        exitGame = false;
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

    void GameInstance::initializeTextures() {
        loadTextureSetFromPath("resources/textures/player", "player");
    }

    void GameInstance::initializeIO() {

    }

    void GameInstance::initializeGameLogic() {
        Game::EntityStats defaultStats;
        Game::Rect defaultHitbox(Game::Vector(0, 0), 100, 100);
        entityTemplates["player"] = Game::EntityTemplate(defaultStats, defaultHitbox, NULL);

        map.createEntity(entityTemplates["player"]);
    }

    void GameInstance::initializeRendering() {
        entityRenderers.push_back(Rendering::EntityRenderer(Rendering::EntityEventParser(&  map, 0), &camera, &window, &textureSets["player"]));
    }

    void GameInstance::initializeGame() {
        initializeWindow();
        initializeTextures();
        initializeIO();
        initializeGameLogic();
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
        for (Rendering::EntityRenderer& currentRenderer : entityRenderers) {
            currentRenderer.updateEntitySprite();
            window.draw(currentRenderer.getSprite());
        }
    }

    void GameInstance::tickGame() {

    }

    void GameInstance::run() {
        initializeGame();
        while (!exitGame) {
            tickIO();
            tickGame();
            tickRendering();
        }
    }

}
