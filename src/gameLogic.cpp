#include "gameLogic.hpp"
#include <iostream>

namespace Game {

    Vector::Vector(int x_a, int y_a) {
        x = x_a;
        y = y_a;
    }

    Vector::Vector(const Vector& vector) {
        x = vector.x;
        y = vector.y;
    }

    Vector::Vector() {
        x = 0;
        y = 0;
    }

    Circle::Circle() {
        center = Vector(0, 0);
        radius = 0;
    }

    Circle::Circle(const Vector& center_a, unsigned int radius_a) {
        center = Vector(center_a);
        radius = radius_a;
    }

    bool Circle::contains(Vector& point) {
        return radius >= sqrt(pow((point.x - center.x),2) + pow((point.y - center.y),2));
    }

    Rect::Rect() {
        topLeft = Vector(0, 0);
        width = 0;
        height = 0;
    }

    Rect::Rect(const Vector& topLeft_a, unsigned int width_a, unsigned int height_a) {
        topLeft = topLeft_a;
        width = width_a;
        height = height_a;
    }

    bool Rect::contains(const Vector& point) const {
        bool x_bound = (point.x >= topLeft.x && point.x <= topLeft.x + width);
        bool y_bound = (point.y >= topLeft.y && point.y <= topLeft.y + height);
        return x_bound && y_bound;
    }

    bool Rect::contains(const Rect& rect) const {
        bool containsTopLeft = contains(rect.topLeft);
        bool containsTopRight = contains(Vector(rect.topLeft.x + rect.width, topLeft.y));
        bool containsBottomLeft = contains(Vector(rect.topLeft.x, rect.topLeft.y + rect.height));
        bool containsBottomRight = contains(Vector(rect.topLeft.x + rect.width, rect.topLeft.y + rect.height));
        return containsTopLeft && containsTopRight && containsBottomLeft && containsBottomRight;
    }

    Game::Vector Rect::getCenter() const {
        if (width <= 0 || height <= 0) {
            return topLeft;
        }
        return Game::Vector(topLeft.x + width / 2, topLeft.y + height / 2);
    }

    bool Rect::intersects(const Rect& rect) const {
        return rect.contains(topLeft) || rect.contains(Vector(topLeft.x + width, topLeft.y)) || rect.contains(Vector(topLeft.x + width, topLeft.y + height)) || rect.contains(Vector(topLeft.x, topLeft.y + height));
    }

    EntityStats::EntityStats() {
        stats["max_hp"] = 1;
        stats["hp"] = 1;
        stats["stam"] = 1;
        stats["sight"] = 1;
        stats["atkSpd"] = 1;

        statModifiers["max_hp"] = 1;
        statModifiers["stamina"] = 1;
        statModifiers["sight"] = 1;
        statModifiers["atkSpd"] = 1;
        statModifiers["move"] = 10;
    }

    EntityStats::EntityStats(const EntityStats& copying) {
        stats = copying.stats;
        statModifiers = copying.statModifiers;
    }

    EntityStats EntityStats::operator+(const EntityStats& adding) {
        EntityStats tempStats(*this);
        for (std::pair<std::string, int> statPair : adding.stats) {
            tempStats.stats[statPair.first] += statPair.second;
        }
        for (std::pair<std::string, int> statPair : adding.statModifiers) {
            tempStats.statModifiers[statPair.first] += statPair.second;
        }
        return tempStats;
    }

    EntityStats EntityStats::operator-(const EntityStats& subtracting) {
        EntityStats tempStats(*this);
        for (std::pair<std::string, int> statPair : subtracting.stats) {
            tempStats.stats[statPair.first] -= statPair.second;
        }
        for (std::pair<std::string, int> statPair : subtracting.statModifiers) {
            tempStats.statModifiers[statPair.first] -= statPair.second;
        }
        return tempStats;
    }

    void EntityStats::operator=(const EntityStats& copying) {
        stats = copying.stats;
        statModifiers = copying.statModifiers;
    }

    void EntityStats::operator+=(const EntityStats& adding) {
        for (std::pair<std::string, int> statPair : adding.stats) {
            stats[statPair.first] += statPair.second;
        }
        for (std::pair<std::string, int> statPair : adding.statModifiers) {
            statModifiers[statPair.first] += statPair.second;
        }
    }

    void EntityStats::operator-=(const EntityStats& subtracting) {
        for (std::pair<std::string, int> statPair : subtracting.stats) {
            stats[statPair.first] -= statPair.second;
        }
        for (std::pair<std::string, int> statPair : subtracting.statModifiers) {
            statModifiers[statPair.first] -= statPair.second;
        }
    }

    MovementMods::MovementMods() {
        ghosting = false;
        noclip = false;
        flying = false;
    }

    BehaviourProfile::BehaviourProfile() {

    }

    unsigned int Buff::getFramesLeft() const {
        return framesLeft;
    }

    unsigned int Buff::getMaxFrames() const {
        return framesMax;
    }

    EntityTemplate::EntityTemplate() {
        stats = EntityStats();
        hitbox = Rect();
    }

    EntityTemplate::EntityTemplate(const EntityStats& stats_, const Rect& hitbox_, BehaviourProfile* behaviourProfile_) {
        stats = stats_;
        hitbox = hitbox_;
        behaviourProfile = behaviourProfile_;
    }

    EntityTemplate::EntityTemplate(const EntityTemplate& copying) {
        stats = copying.stats;
        hitbox = copying.hitbox;
        behaviourProfile = copying.behaviourProfile;
    }

    NoTargeting::NoTargeting() {

    }

    std::vector<Entity*>& NoTargeting::isInRange(std::vector<Entity*>& entities) {
        entities.clear();
        return entities;
    }

    AllTargeting::AllTargeting() {

    }

    std::vector<Entity*>& AllTargeting::isInRange(std::vector<Entity*>& entities) {
        return entities;
    }

    RectTargeting::RectTargeting(const Rect& rect_) {
        rect = rect_;
    }

    std::vector<Entity*>& RectTargeting::isInRange(std::vector<Entity*>& entities) {
        for (std::vector<Entity*>::iterator currentEntity = entities.begin(); currentEntity != entities.begin(); currentEntity++) {
            if (!rect.intersects((*currentEntity)->getHitbox())) {
                currentEntity = entities.erase(currentEntity);
            }
        }
        return entities;
    }

    unsigned int Action::getFrameWait() {
        return frameWait;
    }

    void Action::tickFrameWait() {
        frameWait -= 1;
    }

    HitAction::HitAction(unsigned int damage_) {
        damage = damage_;
    }

    void HitAction::applyAction(std::vector<Entity*>& entities) {
        std::vector<Entity*>& targetableEntities = targeting->isInRange(entities);
        for (std::vector<Entity*>::iterator currentEntity = targetableEntities.begin(); currentEntity != targetableEntities.end(); currentEntity++) {
            EntityStats newStats = EntityStats((*currentEntity)->getBaseStats());
            newStats.stats["hp"] -= damage;
            (*currentEntity)->setStats(newStats);
        }
    }

    HealAction::HealAction(unsigned int healAmount_) {
        healAmount = healAmount_;
    }

    void HealAction::applyAction(std::vector<Entity*>& entities) {
        std::vector<Entity*>& targetableEntities = targeting->isInRange(entities);
        for (std::vector<Entity*>::iterator currentEntity = targetableEntities.begin(); currentEntity != targetableEntities.end(); currentEntity++) {
            EntityStats newStats = EntityStats((*currentEntity)->getBaseStats());
            newStats.stats["hp"] += healAmount;
            (*currentEntity)->setStats(newStats);
        }
    }

    DisplacementAction::DisplacementAction(const Vector& displaceBy_) {
        displaceBy = displaceBy_;
    }

    void DisplacementAction::applyAction(std::vector<Entity*>& entities) {
        std::vector<Entity*>& targetableEntities = targeting->isInRange(entities);
        for (std::vector<Entity*>::iterator currentEntity = targetableEntities.begin(); currentEntity != targetableEntities.end(); currentEntity++) {
            (*currentEntity)->move(displaceBy);
        }
    }

    Entity::Entity(const EntityTemplate& entityTemplate, unsigned int id_, Map* owner) {
        id = id_;
        hitbox = entityTemplate.hitbox;
        baseStats = entityTemplate.stats;
        behaviourProfile = entityTemplate.behaviourProfile;
        ownerMap = owner;
    }

    const unsigned int Entity::getID() {
        return id;
    }

    const Rect Entity::getHitbox() {
        return hitbox;
    }

    void Entity::setHitbox(const Rect& newHitbox) {
        hitbox = newHitbox;
    }

    void Entity::move(const Vector& moveBy) {
        int newX = hitbox.topLeft.x + moveBy.x * getFinalStats().statModifiers["move"];
        int newY = hitbox.topLeft.y + moveBy.y * getFinalStats().statModifiers["move"];
        Rect newHitbox = Rect(Vector(newX, newY), hitbox.width, hitbox.height);
        if (ownerMap->entityCanMoveToSpace(id, newHitbox)) {
            hitbox = newHitbox;
        }
    }

    void Entity::moveWithoutModifier(const Vector& moveBy) {
        int newX = hitbox.topLeft.x + moveBy.x;
        int newY = hitbox.topLeft.y + moveBy.y;
        Rect newHitbox = Rect(Vector(newX, newY), hitbox.width, hitbox.height);
        if (ownerMap->entityCanMoveToSpace(id, newHitbox)) {
            hitbox = newHitbox;
        }
    }

    void Entity::addBuff(std::unique_ptr<Buff>& buff) {
        buffs.push_back(std::move(buff));
    }

    const EntityStats& Entity::getBaseStats() {
        return baseStats;
    }

    void Entity::setStats(const EntityStats& stats) {
        baseStats = stats;
    }

    EntityStats Entity::getFinalStats() {
        EntityStats tempStats = EntityStats(baseStats);
        for (std::unique_ptr<Buff>& buff : buffs) {
            buff->apply(tempStats);
        }
        return tempStats;
    }

    EntityTemplate Entity::getState() {
        EntityTemplate returnTemplate;
        returnTemplate.stats = baseStats;
        returnTemplate.hitbox = hitbox;
        returnTemplate.behaviourProfile = behaviourProfile;
        return returnTemplate;
    }

    void Map::addActionToQueue(Action* action) {
        actions.push_back(action);
    }

    Map::Map() {
        currentMaxID = 0;
        std::vector<Entity*> entities = std::vector<Entity*>();
        std::vector<Action*> actions = std::vector<Action*>();
        playableArea = Rect(Vector(0, 0), 0, 0);
    }

    Map::~Map() {
        for (Entity* currentEntity : entities) {
            free(currentEntity);
        }
        for (Action* currentAction : actions) {
            free(currentAction);
        }
    }

    void Map::tickAndApplyActions() {
        for (std::vector<Action*>::iterator currentAction = actions.begin(); currentAction != actions.end(); currentAction++) {
            if ((*currentAction)->getFrameWait() != 0) {
                (*currentAction)->tickFrameWait();
            }
            else {
                (*currentAction)->applyAction(entities);
                free (*currentAction);
                currentAction = actions.erase(currentAction);
            }
        }

    }

    Entity* Map::getEntityWithID(unsigned int ID) {
        for (Entity* currentEntity : entities) {
            if (currentEntity->getID() == ID) {
                return currentEntity;
            }
        }
        return NULL;
    }

    unsigned int Map::createEntity(const EntityTemplate& entityTemplate) {
        entities.push_back(new Entity(entityTemplate, currentMaxID, this));
        currentMaxID += 1;
        return currentMaxID - 1;
    }

    std::vector<unsigned int> Map::getActiveEntityIDs() {
        std::vector<unsigned int> returnVec;
        for (Game::Entity* currentEntity : entities) {
            returnVec.push_back(currentEntity->getID());
        }
        return returnVec;
    }

    bool Map::spaceEmpty(const Rect& space) {
        bool empty = true;
        for (Entity* currentEntity : entities) {
            if (currentEntity->getHitbox().intersects(space)) {
                empty = false;
            }
        }
        return empty;
    }

    void Map::setPlayableArea(const Rect& playableArea_) {
        playableArea = playableArea_;
    }

    bool Map::entityCanMoveToSpace(unsigned int entityID, const Rect& space) {
        bool moveable = true;
        bool inPlayableArea = playableArea.contains(space);
        for (Entity* currentEntity : entities) {
            bool spaceNotEmpty = currentEntity->getHitbox().intersects(space) && currentEntity->getID() != entityID;
            if (spaceNotEmpty || !inPlayableArea) {
                moveable = false;
            }
        }
        return moveable;
    }

    const std::vector<Entity*>& Map::getEntities() {
        return entities;
    }

}
