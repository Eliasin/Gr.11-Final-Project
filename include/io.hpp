#pragma once
#include <map>
#include "gameLogic.hpp"
#include "rendering.hpp"
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

namespace IO {
    class KeyHandler {
    protected:
        virtual void onKeyPress(sf::Keyboard::Key pressed)=0;
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
        virtual void onKeyPress(sf::Keyboard::Key pressed) override;
    };

    class GameKeyHandler : public KeyHandler {
    protected:
        Game::Map* map;
    };

    class EntityEventKeyHandler : public GameKeyHandler {
        std::map<sf::Keyboard::Key, void (*)(Game::Entity*)> keyEventMap;
        Game::Entity* entity;
    public:
        EntityEventKeyHandler(const std::map<sf::Keyboard::Key, void (*)(Game::Entity*)>& keyEventMap_, Game::Entity* entity_);
        EntityEventKeyHandler(const std::map<sf::Keyboard::Key, void (*)(Game::Entity*)>& keyEventMap_);
        virtual void checkForKeyPress(const sf::Keyboard& keyboard) override;
        virtual void onKeyPress(sf::Keyboard::Key pressed) override;
        Game::Entity* getHandledEntity();
        void setHandledEntity(Game::Entity* entity_);
    };

}
