#pragma once
#include "gameLogic.hpp"
#include <SFML/Graphics.hpp>

namespace Rendering {
    class Camera {
        Game::Vector pos;
    public:
        void move(Game::Vector moveBy);
        void setPos(Game::Vector newPos);
        sf::Vector2<float> translate(Game::Vector gameCoords);
    };

    sf::Vector2<float> scaleSpriteRelativeToWindow(const sf::Sprite& sprite, const sf::Window& window, const sf::Vector2<float>& size);

}
