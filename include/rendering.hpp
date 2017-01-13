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
        Game::Vector reverseTranslate(const sf::Vector2<float>& displayCoords);
        Game::Vector reverseTranslate(const sf::Vector2<int>& displayCoords);
        sf::Vector2<float> scaleSpriteToMatcHitbox(const sf::Sprite& sprite, const sf::Window& window, const Game::Rect& hitbox);
    };

    class Animation {
        std::vector<sf::Texture>* animationSet;
        Camera* camera;
        sf::Window* window;
        sf::Sprite sprite;
        Game::Rect renderbox;
        unsigned int frameDelay;
        unsigned int currentFrame;
        unsigned int ticksSinceFrameChange;
        bool done;

        void advanceSpriteFrame();
        void scaleSprite();
        void updateSprite();
    public:
        Animation(std::vector<sf::Texture>* animationSet_, Camera* camera_, sf::Window* window_, Game::Rect renderbox_, unsigned int frameDelay_);
        void tick();
        const sf::Sprite& getSprite();
        bool doneAnimating();
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
        std::map<std::string, std::vector<sf::Texture>>* textureSet;
        std::vector<sf::Texture>* currentTextureSet;
        EntityEventParser entityEventParser;
        Camera* camera;
        sf::Window* window;
        EntityEventParser::STATE lastState;
        EntityEventParser::STATE currentlyAnimating;

        unsigned int currentFrame;
        unsigned int frameDelay;
        unsigned int ticksSinceFrameChange;

        void scaleSprite();
        void positionSprite();
        void updateSpriteTexture();
        void tickCurrentAnim();
        void switchAnim(EntityEventParser::STATE newState);
        void tickFrameDelayCounter();
    public:
        static const std::vector<std::string> stateTextureNames;
        EntityRenderer(const EntityEventParser& entityEventParser_, Rendering::Camera* camera_, sf::Window* window, std::map<std::string, std::vector<sf::Texture>>* textureSet_, unsigned int frameDelay);
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

    class AbsoluteBackground {
        std::vector<sf::Texture>* backgroundFrames;
        sf::Sprite sprite;
        sf::Window* window;
        unsigned int currentFrame;
        unsigned int frameDelay;
        unsigned int ticksSinceFrameChange;
        bool looping;
        bool frameAscending;
        void scaleSprite();
        void changeFrame();
        void nextFrame();
        void nextFrameLooping();
    public:
        AbsoluteBackground();
        AbsoluteBackground(const AbsoluteBackground& copying);
        AbsoluteBackground(std::vector<sf::Texture>* backgroundFrames_, sf::Window* window_, unsigned int frameDelay_);
        void setTextureSet(std::vector<sf::Texture>* backgroundFrames_);
        void setWindow(sf::Window* window_);
        void setFrameDelay(unsigned int frameDelay_);
        void setLooping(bool looping_);
        void tick();
        const sf::Sprite& getSprite();
    };

    sf::Vector2<float> scaleSpriteRelativeToWindow(const sf::Sprite& sprite, const sf::Window& window, const sf::Vector2<float>& size);

}
