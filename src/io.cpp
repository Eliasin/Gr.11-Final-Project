#include "io.hpp"
#include "gameLogic.hpp"
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

namespace IO {

    CameraKeyHandler::CameraKeyHandler(const std::vector<sf::Keyboard::Key>& triggerOn_, const Game::Vector& moveBy_, Rendering::Camera* camera_) {
        triggerOn = triggerOn_;
        moveBy = moveBy_;
        camera = camera_;
    }

    CameraKeyHandler::CameraKeyHandler(sf::Keyboard::Key triggerOn_, const Game::Vector& moveBy_, Rendering::Camera* camera_) {
        triggerOn.push_back(triggerOn_);
        moveBy = moveBy_;
        camera = camera_;
    }

    void CameraKeyHandler::checkForKeyPress() {
        for (std::vector<sf::Keyboard::Key>::iterator currentKey = triggerOn.begin(); currentKey!= triggerOn.end(); currentKey++) {
            if (sf::Keyboard::isKeyPressed(*currentKey)) {
                onKeyPress(*currentKey);
            }
        }
    }

    void CameraKeyHandler::onKeyPress(sf::Keyboard::Key pressed) {
        camera->move(moveBy);
    }

    EntityEventKeyHandler::EntityEventKeyHandler(const std::map<sf::Keyboard::Key, void (*)(Game::Entity*)>& keyEventMap_, Game::Entity* entity_) {
        keyEventMap = keyEventMap_;
        entity = entity_;
    }

    EntityEventKeyHandler::EntityEventKeyHandler(const std::map<sf::Keyboard::Key, void (*)(Game::Entity*)>& keyEventMap_) {
        keyEventMap = keyEventMap_;
        entity = NULL;
    }

    void EntityEventKeyHandler::checkForKeyPress() {
        for (std::map<sf::Keyboard::Key, void (*)(Game::Entity*)>::iterator currentKey = keyEventMap.begin(); currentKey!= keyEventMap.end(); currentKey++) {
            if (sf::Keyboard::isKeyPressed(currentKey->first)) {
                onKeyPress(currentKey->first);
            }
        }
    }

    void EntityEventKeyHandler::onKeyPress(sf::Keyboard::Key pressed) {
        keyEventMap[pressed](entity);
    }

    Game::Entity* EntityEventKeyHandler::getHandledEntity() {
        return entity;
    }

    void EntityEventKeyHandler::setHandledEntity(Game::Entity* entity_) {
        entity = entity_;
    }

}
