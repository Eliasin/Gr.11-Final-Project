#pragma once
#include "gameLogic.hpp"
#include <SFML/Graphics.hpp>

namespace Rendering {
    class Camera {
        Game::Vector pos;
    public:
        void move(Game::Vector moveBy);
        void setPos(Game::Vector newPos);
        sf::Vector2<int> translate(Game::Vector gameCoords);
    };
}
