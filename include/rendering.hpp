#pragma once
#include "gameLogic.hpp"
#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

namespace Rendering {

    class Camera {
        Game::Rect viewBox;
    public:
        void move(const Game::Vector& moveBy);
        void setPos(const Game::Vector& newPos);
        void setSize(const Game::Vector& newSize);
        void setViewBox(const Game::Rect& newViewBox);
        void centerOn(const Game::Vector& centeringOn, const sf::Window& window);
        sf::Vector2<float> translate(const Game::Vector& gameCoords);
        sf::Vector2<float> scaleSpriteToMatcHitbox(const sf::Sprite& sprite, const sf::Window& window, const Game::Rect& hitbox);
    };

    class EntityRenderer {
        sf::Sprite sprite;
        std::map<std::string, std::vector<sf::Texture>>* textureSet;
        Game::Entity* watchingEntity;
        Camera* camera;
    public:
        static const std::vector<std::string> stateTextureNames;
        EntityRenderer(Game::Entity* watchingEntity_, Rendering::Camera* camera_);
        void updateEntitySprite();
        void setWatchingEntity(Game::Entity* entity);
        void setCamera(Camera* camera_);
        Game::Entity* getWatchingEntity();
        Camera* getCamera();
        const sf::Sprite& getSprite();
    };

    sf::Vector2<float> scaleSpriteRelativeToWindow(const sf::Sprite& sprite, const sf::Window& window, const sf::Vector2<float>& size);

}
