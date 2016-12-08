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

    void CameraKeyHandler::checkForKeyPress(const sf::Keyboard& keyboard) {
        for (std::vector<sf::Keyboard::Key>::iterator currentKey = triggerOn.begin(); currentKey!= triggerOn.end(); currentKey++) {
            if (keyboard.isKeyPressed(*currentKey)) {
                onKeyPress(*currentKey);
            }
        }
    }

    void CameraKeyHandler::onKeyPress(sf::Keyboard::Key pressed) {
        camera->move(moveBy);
    }

    EntityMovementKeyHandler::EntityMovementKeyHandler(const std::vector<sf::Keyboard::Key>& triggerOn_, const Game::Vector& moveBy_, Game::Entity* entity_) {
        triggerOn = triggerOn_;
        moveBy = moveBy_;
        entity = entity_;
    }

    EntityMovementKeyHandler::EntityMovementKeyHandler(sf::Keyboard::Key triggerOn_, const Game::Vector& moveBy_, Game::Entity* entity_) {
        triggerOn.push_back(triggerOn_);
        moveBy = moveBy_;
        entity = entity_;
    }

    EntityMovementKeyHandler::EntityMovementKeyHandler(sf::Keyboard::Key triggerOn_, const Game::Vector& moveBy_) {
        triggerOn.push_back(triggerOn_);
        moveBy = moveBy_;
        entity = NULL;
    }

    void EntityMovementKeyHandler::checkForKeyPress(const sf::Keyboard& keyboard) {
        for (std::vector<sf::Keyboard::Key>::iterator currentKey = triggerOn.begin(); currentKey!= triggerOn.end(); currentKey++) {
            if (keyboard.isKeyPressed(*currentKey)) {
                onKeyPress(*currentKey);
            }
        }
    }

    void EntityMovementKeyHandler::onKeyPress(sf::Keyboard::Key pressed) {
        entity->move(moveBy);
    }

    Game::Entity* EntityMovementKeyHandler::getHandledEntity() {
        return entity;
    }

    void EntityMovementKeyHandler::setHandledEntity(Game::Entity* entity_) {
        entity = entity_;
    }

}
