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
        bool x_bound = (point.x >= topLeft.x && point.x <= topLeft.x + width) || (point.x <= topLeft.x && point.x >= topLeft.x + width);
        bool y_bound = (point.y >= topLeft.y && point.y <= topLeft.y - height) || (point.y <= topLeft.y && point.y >= topLeft.y - height);
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
        max_health = 0;
        health = 0;
        stamina = 0;
        armor = 0;
        moveSpeed = 0;
        sight = 0;
        attackSpeed = 0;
        max_health_p = 0;
        stamina_p = 0;
        armor_p = 0;
        moveSpeed_p = 0;
        sight_p = 0;
        attackSpeed_p = 0;
    }

    EntityStats::EntityStats(const EntityStats& copying) {
        max_health = copying.max_health;
        health = copying.health;
        stamina = copying.stamina;
        armor = copying.armor;
        moveSpeed = copying.moveSpeed;
        sight = copying.sight;
        attackSpeed = copying.attackSpeed;
        max_health_p = copying.max_health_p;
        stamina_p = copying.stamina_p;
        armor_p = copying.armor_p;
        moveSpeed_p = copying.moveSpeed_p;
        sight_p = copying.sight_p;
        attackSpeed_p = copying.attackSpeed_p;
    }

    EntityStats EntityStats::operator+(const EntityStats& adding) {
        EntityStats tempEntity = EntityStats(*this);
        tempEntity.max_health += adding.max_health;
        tempEntity.health += adding.health;
        tempEntity.stamina += adding.stamina;
        tempEntity.armor += adding.armor;
        tempEntity.moveSpeed += adding.moveSpeed;
        tempEntity.sight += adding.sight;
        tempEntity.attackSpeed += adding.attackSpeed;
        tempEntity.max_health_p += adding.max_health_p;
        tempEntity.stamina_p += adding.stamina_p;
        tempEntity.armor_p += adding.armor_p;
        tempEntity.moveSpeed_p += adding.moveSpeed_p;
        tempEntity.sight_p += adding.sight_p;
        tempEntity.attackSpeed_p += adding.attackSpeed_p;
        return tempEntity;
    }

    EntityStats EntityStats::operator-(const EntityStats& subtracting) {
        EntityStats tempEntity = EntityStats(*this);
        tempEntity.max_health -= subtracting.max_health;
        tempEntity.health -= subtracting.health;
        tempEntity.stamina -= subtracting.stamina;
        tempEntity.armor -= subtracting.armor;
        tempEntity.moveSpeed -= subtracting.moveSpeed;
        tempEntity.sight -= subtracting.sight;
        tempEntity.attackSpeed -= subtracting.attackSpeed;
        tempEntity.max_health_p -= subtracting.max_health_p;
        tempEntity.stamina_p -= subtracting.stamina_p;
        tempEntity.armor_p -= subtracting.armor_p;
        tempEntity.moveSpeed_p -= subtracting.moveSpeed_p;
        tempEntity.sight_p -= subtracting.sight_p;
        tempEntity.attackSpeed_p -= subtracting.attackSpeed_p;
        return tempEntity;
    }

    void EntityStats::operator+=(const EntityStats& adding) {
        max_health += adding.max_health;
        health += adding.health;
        stamina += adding.stamina;
        armor += adding.armor;
        moveSpeed += adding.moveSpeed;
        sight += adding.sight;
        attackSpeed += adding.attackSpeed;
        max_health_p += adding.max_health_p;
        stamina_p += adding.stamina_p;
        armor_p += adding.armor_p;
        moveSpeed_p += adding.moveSpeed_p;
        sight_p += adding.sight_p;
        attackSpeed_p += adding.attackSpeed_p;
    }

    void EntityStats::operator-=(const EntityStats& subtracting) {
        max_health -= subtracting.max_health;
        health -= subtracting.health;
        stamina -= subtracting.stamina;
        armor -= subtracting.armor;
        moveSpeed -= subtracting.moveSpeed;
        sight -= subtracting.sight;
        attackSpeed -= subtracting.attackSpeed;
        max_health_p -= subtracting.max_health_p;
        stamina_p -= subtracting.stamina_p;
        armor_p -= subtracting.armor_p;
        moveSpeed_p -= subtracting.moveSpeed_p;
        sight_p -= subtracting.sight_p;
        attackSpeed_p -= subtracting.attackSpeed_p;
    }

    MovementMods::MovementMods() {
        ghosting = false;
        noclip = false;
        flying = false;
    }

    BehaviourProfile::BehaviourProfile() {

    }

    Buff::Info::Info() {

    }

    Buff::Info::Info(const Info& info_a, const Buff::Type& type) {
        switch (type) {
            case Buff::Type::STATS:
            statChanges = info_a.statChanges;
            break;
            case Buff::Type::MOVEMENT:
            movementChanges = info_a.movementChanges;
            break;
            default:
            break;
        }
    }

    Buff::Buff(const Buff::Info& buffInfo, const Buff::Type& typeInfo, unsigned int frames, unsigned int interval) {
        info = Info(buffInfo, typeInfo);
        type = typeInfo;
        framesMax = frames;
        framesLeft = frames;
        frameInterval = interval;
    }

    Buff::Buff(const Buff& copying) {
        framesLeft = copying.framesLeft;
        framesMax = copying.framesMax;
        frameInterval = copying.frameInterval;
        info = copying.info;
        type = copying.type;
    }

    unsigned int Buff::getFramesLeft() {
        return framesLeft;
    }

    unsigned int Buff::getMaxFrames() {
        return framesMax;
    }

    const Buff::Info& Buff::getInfo() {
        return info;
    }

    Buff::Type Buff::getType() {
        return type;
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
            newStats.health -= damage;
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
            newStats.health += healAmount;
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

    void Entity::setHitbox(Rect new_hitbox) {
        hitbox = new_hitbox;
    }

    void Entity::move(Vector move_by) {
        Rect newHitbox = Rect(Vector(hitbox.topLeft.x + move_by.x, hitbox.topLeft.y + move_by.y), hitbox.width, hitbox.height);
        if (ownerMap->entityCanMoveToSpace(id, newHitbox)) {
            hitbox = newHitbox;
        }
    }

    void Entity::addBuff(const Buff& buff) {
        buffs.push_back(buff);
    }

    const EntityStats& Entity::getBaseStats() {
        return baseStats;
    }

    void Entity::setStats(const EntityStats& stats) {
        baseStats = stats;
    }

    EntityStats Entity::getFinalStats() {
        EntityStats tempStats = EntityStats(baseStats);
        for (std::vector<Buff>::iterator i = buffs.begin(); i != buffs.end(); i++) {
            if (i->getType() == Buff::Type::STATS) {
                tempStats += i->getInfo().statChanges;
            }
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
        bool validPlayableArea = (playableArea.width <= 0 || playableArea.height <= 0);
        bool inPlayableArea = playableArea.contains(space);
        for (Entity* currentEntity : entities) {
            if (currentEntity->getHitbox().intersects(space) && currentEntity->getID() != entityID && inPlayableArea && validPlayableArea) {
                moveable = false;
            }
        }
        return moveable;
    }

    const std::vector<Entity*>& Map::getEntities() {
        return entities;
    }

}
