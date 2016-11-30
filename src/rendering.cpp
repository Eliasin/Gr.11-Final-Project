#include "rendering.hpp"
#include <SFML/Graphics.hpp>


namespace Rendering {

    void Camera::move(Game::Vector moveBy) {
        pos.x += moveBy.x;
        pos.y += moveBy.y;
    }

    void Camera::setPos(Game::Vector newPos) {
        pos = newPos;
    }

    sf::Vector2<float> Camera::translate(Game::Vector gameCoords) {
        return sf::Vector2<float>(gameCoords.x - pos.x, gameCoords.y - pos.y);
    }

    sf::Vector2<float> scaleTextureRelativeToWindow(sf::Sprite sprite, sf::Window window, sf::Vector2<float> size) {
        const sf::Texture* spriteTexture = sprite.getTexture();
        return sf::Vector2<float>(window.getSize().x * size.x / spriteTexture->getSize().x, window.getSize().y * size.y / spriteTexture->getSize().y);
    }

}
