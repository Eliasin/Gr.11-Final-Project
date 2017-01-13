#pragma once
#include <map>
#include <utility>
#include <cmath>
#include <iostream>
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
        virtual void onMouseEvent(sf::Vector2<int> position, sf::Mouse::Button pressed)=0;
    public:
        virtual void checkForMouseEvents()=0;
    };

    class PlayerAttackMouseHandler : public MouseHandler {
    protected:
        Game::Map* map;
        sf::Window* window;
        std::vector<Rendering::Animation>* animations;
        std::vector<sf::Texture>* attackAnimation;
        unsigned int framesSinceAttack;
        unsigned int entityID;
        virtual bool entityValid();
        Rendering::Camera* camera;
        virtual void spawnAttackAction(Game::Vector pos);
        virtual void onMouseEvent(sf::Vector2<int> position, sf::Mouse::Button pressed) override;
    public:
        PlayerAttackMouseHandler(unsigned int entityID_, Game::Map* map_, Rendering::Camera* camera_, sf::Window* window_, std::vector<Rendering::Animation>* animations_, std::vector<sf::Texture>* attackAnimation_);
        virtual void checkForMouseEvents() override;
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
