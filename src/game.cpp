#include "game.hpp"

namespace Main {

    void GameInstance::initializeRenderingAndWindow() {
        window.create(sf::VideoMode::getDesktopMode(), "Gr.11-Final-Project");
        camera.setPos(Game::Vector(0, 0));
    }

    void GameInstance::loadTextureSetFromPath(std::string setPath, std::string name) {
        std::map<std::string, sf::Texture> textureSet;
        for (std::string currentState : Rendering::EntityRenderer::stateTextureNames) {
            sf::Texture texture;
            std::cout << setPath + "/" + currentState + ".png" << std::endl;
            if (!texture.loadFromFile(setPath + "/"  + currentState + ".png")) {
                texture.loadFromFile(setPath + "/idle.png");
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

    }

    void GameInstance::initializeGame() {
        initializeRenderingAndWindow();
        initializeTextures();
        initializeIO();
    }

    void GameInstance::tickGame() {

    }

    void GameInstance::run() {
        initializeGame();
        for (;;) {
            tickGame();
        }
    }

}
