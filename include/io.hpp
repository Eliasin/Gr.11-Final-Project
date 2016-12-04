#pragma once
#include "gameLogic.hpp"
#include "rendering.hpp"
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

namespace IO {
    class KeyHandler {
    protected:
        virtual void onKeyPress()=0;
    public:
        virtual void checkForKeyPress(const sf::Keyboard& keyboard)=0;
    };

    class CameraKeyHandler : public KeyHandler {
        std::vector<sf::Keyboard::Key> triggerOn;
        Game::Vector moveBy;
        Rendering::Camera* camera;
    public:
        CameraKeyHandler(const std::vector<sf::Keyboard::Key>& triggerOn_, const Game::Vector& moveBy_, Rendering::Camera* camera_);
        CameraKeyHandler(sf::Keyboard::Key triggerOn_, const Game::Vector& moveBy_, Rendering::Camera* camera_);
        virtual void checkForKeyPress(const sf::Keyboard& keyboard) override;
        virtual void onKeyPress() override;
    };

    class GameKeyHandler : public KeyHandler {
    protected:
        Game::Map* map;
    };

    class EntityMovementKeyHandler : public GameKeyHandler {
        std::vector<sf::Keyboard::Key> triggerOn;
        Game::Vector moveBy;
        Game::Entity* entity;
    public:
        PlayerMovementKeyHandler(const std::vector<sf::Keyboard::Key>& triggerOn_, const Game::Vector& moveBy_, Game::Entity* entity_);
        PlayerMovementKeyHandler(sf::Keyboard::Key triggerOn_, const Game::Vector& moveBy_, Game::Entity* entity_);
        virtual void checkForKeyPress(const sf::Keyboard& keyboard) override;
        virtual void onKeyPress() override;
    };

}
