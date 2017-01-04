#include "rendering.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>


namespace Rendering {

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
        return scaleSpriteRelativeToWindow(sprite, window, sf::Vector2<float>((float)hitbox.width / (float)viewBox.width, (float)hitbox.height / (float)viewBox.height));        return sf::Vector2<float>(0.f, 0.f);
    }

    sf::Vector2<float> scaleSpriteRelativeToWindow(const sf::Sprite& sprite, const sf::Window& window, const sf::Vector2<float>& size) {
        const sf::Texture* spriteTexture = sprite.getTexture();
        if (!spriteTexture || spriteTexture->getSize().x <= 0 || spriteTexture->getSize().y <= 0) {
            return sf::Vector2<float>(0.f, 0.f);
        }
        float xScale = window.getSize().x * size.x / spriteTexture->getSize().x;
        float yScale = window.getSize().y * size.y / spriteTexture->getSize().y;
        return sf::Vector2<float>(xScale, yScale);
        return sf::Vector2<float>(0, 0);
    }

    EntityEventParser::EntityEventParser(Game::Map* map_, unsigned int entityID_) {
        map = map_;
        entityID = entityID_;
        currentState = STATE::IDLE;

        if (entityValid()) {
            lastState = map_->getEntityWithID(entityID_)->getState();
        }
        else {
            lastState = Game::EntityTemplate();
        }
    }

    EntityEventParser::EntityEventParser(const EntityEventParser& copying) {
        map = copying.map;
        entityID = copying.entityID;
        lastState = copying.lastState;
        currentState = STATE::IDLE;
    }

    EntityEventParser::EntityEventParser() {
        map = NULL;
        entityID = 0;
        currentState = STATE::IDLE;
    }

    unsigned int EntityEventParser::getEntityID() {
        return entityID;
    }

    void EntityEventParser::setEntityID(unsigned int newID) {
        entityID = newID;
    }

    void EntityEventParser::updateCurrentState() {
        if (entityValid()) {
            Game::EntityStats currentEntityState = map->getEntityWithID(entityID)->getState().stats;

            if (lastState.stats.health > currentEntityState.health) {
                currentState = EntityEventParser::STATE::HIT;
            }
            else {
                currentState = EntityEventParser::STATE::IDLE;
            }
        }
    }

    void EntityEventParser::grabEntityState() {
        if (entityValid()) {
            lastState = map->getEntityWithID(entityID)->getState();
        }
    }

    bool EntityEventParser::entityValid() const {
        if (map && map->getEntityWithID(entityID)) {
            return true;
        }
        return false;
    }

    EntityEventParser::STATE EntityEventParser::getEntityState() {
        grabEntityState();
        updateCurrentState();
        return currentState;
    }

    Game::Rect EntityEventParser::getEntityHitbox() {
        if (entityValid()) {
            return map->getEntityWithID(entityID)->getHitbox();
        }
        return Game::Rect(Game::Vector(0, 0), 1, 1);
    }

    const std::vector<std::string> EntityRenderer::stateTextureNames = { "idle", "hit" };

    const std::map <EntityEventParser::STATE, std::string> EntityRenderer::stateToTextureName = { {EntityEventParser::STATE::HIT, "hit"}, {EntityEventParser::STATE::IDLE, "idle"} };

    void EntityRenderer::initializeSprite() {
        sprite.setTexture((*textureSet)["idle"]);
    }

    EntityRenderer::EntityRenderer(const EntityEventParser& entityEventParser_, Rendering::Camera* camera_, sf::Window* window_, std::map<std::string, sf::Texture>* textureSet_) {
        entityEventParser = EntityEventParser(entityEventParser_);
        camera = camera_;
        textureSet = textureSet_;
        window = window_;
        lastState = EntityEventParser::STATE::IDLE;
        sprite.setTexture((*textureSet)["idle"]);
        scaleSprite();
    }

    void EntityRenderer::scaleSprite() {
        if (entityEventParser.entityValid()) {
            sf::Vector2<float> newScale = camera->scaleSpriteToMatcHitbox(sprite, *window, entityEventParser.getEntityHitbox());
            sprite.setScale(newScale);
        }
    }

    void EntityRenderer::positionSprite() {
        sf::Vector2<float> position = camera->translate(entityEventParser.getEntityHitbox().topLeft);
        sprite.setPosition(position);
    }

    void EntityRenderer::updateSpriteTexture() {
        EntityEventParser::STATE entityState = entityEventParser.getEntityState();
        if (lastState != entityState) {
            std::string textureName = stateToTextureName.find(entityState)->second;
            sprite.setTexture(textureSet->find(textureName)->second);
        }
    }

    void EntityRenderer::updateEntitySprite() {
        if (entityEventParser.entityValid()) {
            positionSprite();
            updateSpriteTexture();
        }
    }

    void EntityRenderer::setCamera(Camera* camera_) {
        camera = camera_;
    }

    const EntityEventParser& EntityRenderer::getEntityEventParser() {
        return entityEventParser;
    }

    Camera* EntityRenderer::getCamera() {
        return camera;
    }

    const sf::Sprite& EntityRenderer::getSprite() {
        return sprite;
    }

}
