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
                onKeyPress();
            }
        }
    }

    void CameraKeyHandler::onKeyPress() {
        camera->move(moveBy);
    }

    EntityMovementKeyHandler::PlayerMovementKeyHandler(const std::vector<sf::Keyboard::Key>& triggerOn_, const Game::Vector& moveBy_, Game::Entity* entity_) {
        triggerOn = triggerOn_;
        moveBy = moveBy_;
        entity = entity_;
    }

    EntityMovementKeyHandler::PlayerMovementKeyHandler(sf::Keyboard::Key triggerOn_, const Game::Vector& moveBy_, Game::Entity* entity_) {
        triggerOn.push_back(triggerOn_);
        moveBy = moveBy_;
        entity = entity_;
    }

    void EntityMovementKeyHandler::checkForKeyPress(const sf::Keyboard& keyboard) {
        for (std::vector<sf::Keyboard::Key>::iterator currentKey = triggerOn.begin(); currentKey!= triggerOn.end(); currentKey++) {
            if (keyboard.isKeyPressed(*currentKey)) {
                onKeyPress();
            }
        }
    }

    void EntityMovementKeyHandler::onKeyPress() {
        entity->move(moveBy);
    }

}
