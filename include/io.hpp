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
        virtual void checkForKeyPress()=0;
    };

    class MouseHandler {
    protected:
        virtual void onMouseButtonPress(sf::Mouse::Button pressed)=0;
        virtual void onMouseMove(sf::Vector2<int> position)=0;
    };

    class GameKeyHandler : public KeyHandler {
    protected:
        Game::Map* map;
    };

    class EntityMovementKeyHandler : public GameKeyHandler {
    protected:
        unsigned int entityID;
        std::map<sf::Keyboard::Key, Game::Vector> keyMovementMap;
        virtual void onKeyPress(sf::Keyboard::Key pressed) override;
        bool entityValid();
    public:
        EntityMovementKeyHandler(const std::map<sf::Keyboard::Key, Game::Vector>& keyMovementMap_, Game::Map* map_, unsigned int entityID_);
        virtual void checkForKeyPress() override;
        unsigned int getHandlingEntityID();
    };

}
