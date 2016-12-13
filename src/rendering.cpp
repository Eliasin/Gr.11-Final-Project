#include "rendering.hpp"
#include <SFML/Graphics.hpp>


namespace Rendering {

    const std::vector<std::string> RenderProfile::stateTextureNames = { "idle", "hurt" };

    void Camera::move(const Game::Vector& moveBy) {
        viewBox.topLeft.x += moveBy.x;
        viewBox.topLeft.y += moveBy.y;
    }

    void Camera::setPos(const Game::Vector& newPos) {
        viewBox.topLeft = newPos;
    }

    void Camera::setSize(const Game::Vector& newSize) {
        viewBox.width = newSize.x;
        viewBox.height = newSize.y;
    }

    void Camera::setViewBox(const Game::Rect& newViewBox) {
        viewBox = newViewBox;
    }

    sf::Vector2<float> Camera::translate(const Game::Vector& gameCoords) {
        return sf::Vector2<float>(gameCoords.x - viewBox.topLeft.x, gameCoords.y - viewBox.topLeft.y);
    }

    void Camera::centerOn(const Game::Vector& centeringOn, const sf::Window& window) {
        setPos(Game::Vector(centeringOn.x - window.getSize().x / 2, centeringOn.y - window.getSize().y / 2));
    }

    sf::Vector2<float> Camera::scaleSpriteToMatcHitbox(const sf::Sprite& sprite, const sf::Window& window, const Game::Rect& hitbox) {
        scaleSpriteRelativeToWindow(sprite, window, sf::Vector2<float>((float)hitbox.width / (float)viewBox.width, (float)hitbox.height / (float)viewBox.height));
    }

    sf::Vector2<float> scaleSpriteRelativeToWindow(const sf::Sprite& sprite, const sf::Window& window, const sf::Vector2<float>& size) {
        const sf::Texture* spriteTexture = sprite.getTexture();
        return sf::Vector2<float>(window.getSize().x * size.x / spriteTexture->getSize().x, window.getSize().y * size.y / spriteTexture->getSize().y);
    }

}
