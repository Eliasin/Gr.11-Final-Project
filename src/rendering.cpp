#include "rendering.hpp"
#include <SFML/Graphics.hpp>


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
        scaleSpriteRelativeToWindow(sprite, window, sf::Vector2<float>((float)hitbox.width / (float)viewBox.width, (float)hitbox.height / (float)viewBox.height));
    }

    sf::Vector2<float> scaleSpriteRelativeToWindow(const sf::Sprite& sprite, const sf::Window& window, const sf::Vector2<float>& size) {
        const sf::Texture* spriteTexture = sprite.getTexture();
        return sf::Vector2<float>(window.getSize().x * size.x / spriteTexture->getSize().x, window.getSize().y * size.y / spriteTexture->getSize().y);
    }

    EntityEventParser::EntityEventParser(Game::Map* map_, unsigned int entityID_) {
        map = map_;
        entityID = entityID_;
        lastState = map_->getEntityWithID(entityID_)->getState();
        currentState = STATE::IDLE;
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
        if (!map->getEntityWithID(entityID)) {
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
        if (map->getEntityWithID(entityID)) {
            lastState = map->getEntityWithID(entityID)->getState();
        }
    }

    EntityEventParser::STATE EntityEventParser::getEntityState() {
        grabEntityState();
        updateCurrentState();
        return currentState;
    }

    Game::Rect EntityEventParser::getEntityHitbox() {
        return map->getEntityWithID(entityID)->getHitbox();
    }

    const std::vector<std::string> EntityRenderer::stateTextureNames = { "idle", "hit" };

    const std::map <EntityEventParser::STATE, std::string> EntityRenderer::stateToTextureName = { {EntityEventParser::STATE::HIT, "hit"}, {EntityEventParser::STATE::IDLE, "idle"} };

    void EntityRenderer::initializeSprite() {
        sprite.setTexture((*textureSet)["idle"]);
    }

    EntityRenderer::EntityRenderer(const EntityEventParser& entityEventParser_, Rendering::Camera* camera_, sf::Window* window_, std::map<std::string, sf::Texture>* textureSet_) {
        entityEventParser = entityEventParser_;
        camera = camera_;
        textureSet = textureSet_;
        window = window_;
        lastState = EntityEventParser::STATE::IDLE;
        scaleSprite();
    }

    void EntityRenderer::scaleSprite() {
        sf::Vector2<float> newScale = camera->scaleSpriteToMatcHitbox(sprite, *window, entityEventParser.getEntityHitbox());
        sprite.setScale(newScale);
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
        positionSprite();
        updateSpriteTexture();
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
