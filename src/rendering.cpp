#include "rendering.hpp"

namespace Rendering {

    void Camera::move(Game::Vector moveBy) {
        pos.x += moveBy.x;
        pos.y += moveBy.y;
    }

    void Camera::setPos(Game::Vector newPos) {
        pos = newPos;
    }

    sf::Vector2<int> Camera::translate(Game::Vector gameCoords) {
        return sf::Vector2<int>(gameCoords.x - pos.x, gameCoords.y - pos.y);
    }

}
