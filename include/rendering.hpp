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

    class EntityEventParser {
    public:
        enum class STATE {
            HIT,
            IDLE
        };
    private:
        unsigned int entityID;
        Game::Map* map;
        Game::EntityTemplate lastState;
        STATE currentState;
    public:
        EntityEventParser(Game::Map* map_, unsigned int entityID_);
        EntityEventParser(const EntityEventParser& copying);
        EntityEventParser();
        void updateCurrentState();
        void grabEntityState();
        bool entityValid() const;
        STATE getEntityState();
        Game::Rect getEntityHitbox();
        unsigned int getEntityID();
        void setEntityID(unsigned int newID);
    };

    class EntityRenderer {
        static const std::map<EntityEventParser::STATE, std::string> stateToTextureName;
        sf::Sprite sprite;
        std::map<std::string, sf::Texture>* textureSet;
        EntityEventParser entityEventParser;
        Camera* camera;
        sf::Window* window;
        EntityEventParser::STATE lastState;

        void scaleSprite();
        void positionSprite();
        void updateSpriteTexture();

        void initializeSprite();
    public:
        static const std::vector<std::string> stateTextureNames;
        EntityRenderer(const EntityEventParser& entityEventParser_, Rendering::Camera* camera_, sf::Window* window, std::map<std::string, sf::Texture >* textureSet_);
        void updateEntitySprite();
        void setCamera(Camera* camera_);
        const EntityEventParser& getEntityEventParser();
        Camera* getCamera();
        const sf::Sprite& getSprite();
    };

    class Background {
        sf::Texture* backgroundTexture;
        sf::Sprite sprite;
        Camera* camera;
        sf::Window* window;
        Game::Rect renderZone;
    public:
        Background(sf::Texture* backgroundTexture_, Camera* camera_, sf::Window* window_, const Game::Rect& renderZone_);
        void updateBackgroundSprite();
        void setBackgroundTexture(sf::Texture* backgroundTexture_);
        const sf::Sprite& getSprite();
    };

    sf::Vector2<float> scaleSpriteRelativeToWindow(const sf::Sprite& sprite, const sf::Window& window, const sf::Vector2<float>& size);

}
