#include "io.hpp"
#include "gameLogic.hpp"
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

namespace IO {

    EntityMovementKeyHandler::EntityMovementKeyHandler(const std::map<sf::Keyboard::Key, Game::Vector>& keyMovementMap_, Game::Map* map_, unsigned int entityID_) {
        keyMovementMap = keyMovementMap_;
        map = map_;
        entityID = entityID_;
    }

    bool EntityMovementKeyHandler::entityValid() {
        if (map && map->getEntityWithID(entityID)) {
            return true;
        }
        return false;
    }

    void EntityMovementKeyHandler::checkForKeyPress() {
        for (std::pair<sf::Keyboard::Key, Game::Vector> keyVectorPair : keyMovementMap) {
            if (sf::Keyboard::isKeyPressed(keyVectorPair.first)) {
                onKeyPress(keyVectorPair.first);
            }
        }
    }

    void EntityMovementKeyHandler::onKeyPress(sf::Keyboard::Key pressed) {
        if (entityValid()) {
            map->getEntityWithID(entityID)->move(keyMovementMap[pressed]);
        }
    }

}
