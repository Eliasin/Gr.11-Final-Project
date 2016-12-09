#include "game.hpp"

namespace Main {

    void GameInstance::initializeGame() {
        window = sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Gr.11-Final-Project")
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
