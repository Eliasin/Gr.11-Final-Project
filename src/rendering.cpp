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

    Game::Vector Camera::reverseTranslate(const sf::Vector2<float>& displayCoords) {
        return Game::Vector(displayCoords.x + viewBox.topLeft.x, displayCoords.y + viewBox.topLeft.y);
    }

    Game::Vector Camera::reverseTranslate(const sf::Vector2<int>& displayCoords) {
        return Game::Vector(displayCoords.x + viewBox.topLeft.x, displayCoords.y + viewBox.topLeft.y);
    }

    void Camera::centerOn(const Game::Vector& centeringOn, const sf::Window& window) {
        setPos(Game::Vector(centeringOn.x - window.getSize().x / 2, centeringOn.y - window.getSize().y / 2));
    }

    sf::Vector2<float> Camera::scaleSpriteToMatcHitbox(const sf::Sprite& sprite, const sf::Window& window, const Game::Rect& hitbox) {
        return scaleSpriteRelativeToWindow(sprite, window, sf::Vector2<float>((float)hitbox.width / (float)viewBox.width, (float)hitbox.height / (float)viewBox.height));
    }

    sf::Vector2<float> scaleSpriteRelativeToWindow(const sf::Sprite& sprite, const sf::Window& window, const sf::Vector2<float>& size) {
        const sf::Texture* spriteTexture = sprite.getTexture();
        if (!spriteTexture || spriteTexture->getSize().x <= 0 || spriteTexture->getSize().y <= 0) {
            return sf::Vector2<float>(0.f, 0.f);
        }
        float xScale = window.getSize().x * size.x / spriteTexture->getSize().x;
        float yScale = window.getSize().y * size.y / spriteTexture->getSize().y;
        return sf::Vector2<float>(xScale, yScale);
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

            if (lastState.stats.stats[Game::EntityStats::STAT::HP] > currentEntityState.stats[Game::EntityStats::STAT::HP]) {
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
        updateCurrentState();
        grabEntityState();
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

    EntityRenderer::EntityRenderer(const EntityEventParser& entityEventParser_, Rendering::Camera* camera_, sf::Window* window_, std::map<std::string, std::vector<sf::Texture>>* textureSet_, unsigned int frameDelay_) {
        entityEventParser = EntityEventParser(entityEventParser_);
        camera = camera_;
        textureSet = textureSet_;
        window = window_;
        lastState = EntityEventParser::STATE::IDLE;
        currentTextureSet = &(*textureSet)["idle"];
        frameDelay = frameDelay_;
        currentFrame = 0;
        ticksSinceFrameChange = 0;
        currentlyAnimating = EntityEventParser::STATE::IDLE;
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

    void EntityRenderer::switchAnim(EntityEventParser::STATE newState) {
        currentTextureSet = &(*textureSet)[stateToTextureName.find(newState)->second];
        currentlyAnimating = newState;
        currentFrame = 0;
        ticksSinceFrameChange = 0;
    }

    void EntityRenderer::tickCurrentAnim() {
        if (currentFrame < currentTextureSet->size() - 1) {
            currentFrame++;
        }
        else {
            switchAnim(EntityEventParser::STATE::IDLE);
        }
        sprite.setTexture((*currentTextureSet)[currentFrame]);
        scaleSprite();
    }

    void EntityRenderer::tickFrameDelayCounter() {
        ticksSinceFrameChange++;
        if (ticksSinceFrameChange > frameDelay) {
            tickCurrentAnim();
            ticksSinceFrameChange = 0;
        }
    }

    void EntityRenderer::updateSpriteTexture() {
        EntityEventParser::STATE entityState = entityEventParser.getEntityState();
        if (lastState != entityState && entityState != EntityEventParser::STATE::IDLE && entityState != currentlyAnimating) {
            switchAnim(entityState);
        }
        else {
            tickFrameDelayCounter();
        }
        lastState = entityState;
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

    Background::Background(sf::Texture* backgroundTexture_, Camera* camera_, sf::Window* window_, const Game::Rect& renderZone_) {
        backgroundTexture = backgroundTexture_;
        camera = camera_;
        window = window_;
        renderZone = renderZone_;
        sprite.setTexture(*backgroundTexture);
    }

    void Background::setBackgroundTexture(sf::Texture* backgroundTexture_) {
        backgroundTexture = backgroundTexture_;
    }

    void Background::updateBackgroundSprite() {
        sf::Rect<int> newTextureRect = sprite.getTextureRect();
        sf::Vector2<float> rectScale = camera->scaleSpriteToMatcHitbox(sprite, *window, renderZone);
        newTextureRect.width = (float)newTextureRect.width * rectScale.x;
        newTextureRect.height = (float)newTextureRect.height * rectScale.y;
        sprite.setTextureRect(newTextureRect);
        sprite.setPosition(camera->translate(renderZone.topLeft));
    }

    const sf::Sprite& Background::getSprite() {
        return sprite;
    }

    AbsoluteBackground::AbsoluteBackground() {
        backgroundFrames = NULL;
        currentFrame = 0;
        window = NULL;
        frameDelay = 1;
        ticksSinceFrameChange = 0;
        looping = false;
        frameAscending = true;
    }

    AbsoluteBackground::AbsoluteBackground(const AbsoluteBackground& copying) {
        backgroundFrames = copying.backgroundFrames;
        currentFrame = copying.currentFrame;
        window = copying.window;
        frameDelay = copying.frameDelay;
        ticksSinceFrameChange = copying.ticksSinceFrameChange;
        looping = copying.looping;
        frameAscending = copying.frameAscending;
    }

    AbsoluteBackground::AbsoluteBackground(std::vector<sf::Texture>* backgroundFrames_, sf::Window* window_, unsigned int frameDelay_) {
        backgroundFrames = backgroundFrames_;
        currentFrame = 0;
        window = window_;
        frameDelay = frameDelay_;
        ticksSinceFrameChange = 0;
        looping = false;
        frameAscending = true;
    }

    void AbsoluteBackground::setTextureSet(std::vector<sf::Texture>* backgroundFrames_) {
        backgroundFrames = backgroundFrames_;
    }

    void AbsoluteBackground::setWindow(sf::Window* window_) {
        window = window_;
    }

    void AbsoluteBackground::setFrameDelay(unsigned int frameDelay_) {
        frameDelay = frameDelay_;
    }

    void AbsoluteBackground::setLooping(bool looping_) {
        looping = looping_;
    }

    void AbsoluteBackground::scaleSprite() {
        sprite.setScale(scaleSpriteRelativeToWindow(sprite, *window, sf::Vector2<float>(1.f, 1.f)));
    }

    void AbsoluteBackground::changeFrame() {
        if (looping) {
            nextFrameLooping();
        }
        else {
            nextFrame();
        }
    }

    void AbsoluteBackground::nextFrame() {
        if (currentFrame < backgroundFrames->size() - 1) {
            currentFrame++;
        }
        else {
            currentFrame = 0;
        }
    }

    void AbsoluteBackground::nextFrameLooping() {
        if (frameAscending) {
            if (currentFrame < backgroundFrames->size() - 1) {
                currentFrame++;
            }
            else {
                frameAscending = false;
            }
        }
        else {
            if (currentFrame > 0) {
                currentFrame--;
            }
            else {
                frameAscending = true;
            }
        }
    }

    void AbsoluteBackground::tick() {
        if (ticksSinceFrameChange < frameDelay) {
            ticksSinceFrameChange++;
        }
        else {
            changeFrame();
            ticksSinceFrameChange = 0;
        }

        if (!backgroundFrames->empty()) {
            sprite.setTexture((*backgroundFrames)[currentFrame]);
            scaleSprite();
        }
    }

    const sf::Sprite& AbsoluteBackground::getSprite() {
        return sprite;
    }

}
