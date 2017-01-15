#include "gameLogic.hpp"
#include <iostream>

namespace Game {

    float manhattanDistance(Game::Vector p1, Game::Vector p2) {
        return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
    }

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

    Vector rotatePoint(Vector point, Vector anchor, float angle) {
        Vector rotated;
        rotated.x = cos(angle*PI/180) * (point.x - anchor.x) - sin(angle*PI/180) * (point.y - anchor.y) + anchor.x;
        rotated.y = sin(angle*PI/180) * (point.x - anchor.x) + cos(angle*PI/180) * (point.y - anchor.y) + anchor.y;
        return rotated;
    }

    Circle::Circle() {
        center = Vector(0, 0);
        radius = 0;
    }

    Circle::Circle(const Vector& center_a, unsigned int radius_a) {
        center = Vector(center_a);
        radius = radius_a;
    }

    bool Circle::intersects(const Rect& rect) const {
        bool containsTopLeft = contains(rect.topLeft);
        bool containsTopRight = contains(Vector(rect.topLeft.x + rect.width, rect.topLeft.y));
        bool containsBottomLeft = contains(Vector(rect.topLeft.x, rect.topLeft.y + rect.height));
        bool containsBottomRight = contains(Vector(rect.topLeft.x + rect.width, rect.topLeft.y + rect.height));
        return containsTopLeft || containsTopRight || containsBottomLeft || containsBottomRight;
    }

    bool Circle::contains(const Vector& point) const {
        return radius >= sqrt(pow((point.x - center.x),2) + pow((point.y - center.y),2));
    }

    bool Circle::contains(const Rect& rect) const {
        bool containsTopLeft = contains(rect.topLeft);
        bool containsTopRight = contains(Vector(rect.topLeft.x + rect.width, rect.topLeft.y));
        bool containsBottomLeft = contains(Vector(rect.topLeft.x, rect.topLeft.y + rect.height));
        bool containsBottomRight = contains(Vector(rect.topLeft.x + rect.width, rect.topLeft.y + rect.height));
        return containsTopLeft && containsTopRight && containsBottomLeft && containsBottomRight;
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
        bool containsTopRight = contains(Vector(rect.topLeft.x + rect.width, rect.topLeft.y));
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
        bool containsTopLeft = contains(rect.topLeft);
        bool containsTopRight = contains(Vector(rect.topLeft.x + rect.width, rect.topLeft.y));
        bool containsBottomLeft = contains(Vector(rect.topLeft.x, rect.topLeft.y + rect.height));
        bool containsBottomRight = contains(Vector(rect.topLeft.x + rect.width, rect.topLeft.y + rect.height));

        bool containedTopLeft = rect.contains(topLeft);
        bool containedTopRight = rect.contains(Vector(topLeft.x + width, topLeft.y));
        bool containedBottomLeft = rect.contains(Vector(topLeft.x, topLeft.y + height));
        bool containedBottomRight = rect.contains(Vector(topLeft.x + width, topLeft.y + height));

        bool containsOtherRect = containsTopLeft || containsTopRight || containsBottomLeft || containsBottomRight;
        bool containedByOtherRect = containedTopLeft || containedTopRight || containedBottomLeft || containedBottomRight;

        return containsOtherRect || containedByOtherRect;
    }

    bool Rect::operator==(const Rect& rect) const {
        return topLeft.x == rect.topLeft.x && topLeft.y == rect.topLeft.y && width == rect.width && height == rect.height;
    }

    EntityStats::EntityStats() {
        stats[STAT::MAX_HP] = 50;
        stats[STAT::HP] = 50;
        stats[STAT::MAX_STAM] = 1;
        stats[STAT::STAM] = 1;
        stats[STAT::SIGHT] = 1;
        stats[STAT::ATK_DELAY] = 15;
        stats[STAT::RNG] = 150;
        stats[STAT::DMG] = 1;

        statModifiers[STAT_MOD::MAX_HP] = 1;
        statModifiers[STAT_MOD::MAX_STAM] = 1;
        statModifiers[STAT_MOD::SIGHT] = 1;
        statModifiers[STAT_MOD::ATK_DELAY] = 1;
        statModifiers[STAT_MOD::MOVE] = 10;
        statModifiers[STAT_MOD::DMG] = 1;
    }

    EntityStats::EntityStats(const EntityStats& copying) {
        stats = copying.stats;
        statModifiers = copying.statModifiers;
    }

    EntityStats EntityStats::operator+(const EntityStats& adding) {
        EntityStats tempStats(*this);
        for (std::pair<STAT, int> statPair : adding.stats) {
            tempStats.stats[statPair.first] += statPair.second;
        }
        for (std::pair<STAT_MOD, int> statPair : adding.statModifiers) {
            tempStats.statModifiers[statPair.first] += statPair.second;
        }
        return tempStats;
    }

    EntityStats EntityStats::operator-(const EntityStats& subtracting) {
        EntityStats tempStats(*this);
        for (std::pair<STAT, int> statPair : subtracting.stats) {
            tempStats.stats[statPair.first] -= statPair.second;
        }
        for (std::pair<STAT_MOD, int> statPair : subtracting.statModifiers) {
            tempStats.statModifiers[statPair.first] -= statPair.second;
        }
        return tempStats;
    }

    void EntityStats::operator=(const EntityStats& copying) {
        stats = copying.stats;
        statModifiers = copying.statModifiers;
    }

    void EntityStats::operator+=(const EntityStats& adding) {
        for (std::pair<STAT, int> statPair : adding.stats) {
            stats[statPair.first] += statPair.second;
        }
        for (std::pair<STAT_MOD, int> statPair : adding.statModifiers) {
            statModifiers[statPair.first] += statPair.second;
        }
    }

    void EntityStats::operator-=(const EntityStats& subtracting) {
        for (std::pair<STAT, int> statPair : subtracting.stats) {
            stats[statPair.first] -= statPair.second;
        }
        for (std::pair<STAT_MOD, int> statPair : subtracting.statModifiers) {
            statModifiers[statPair.first] -= statPair.second;
        }
    }

    bool BehaviourProfile::entityValid() const {
        return map && map->getEntityWithID(entityID);
    }

    BehaviourProfile::Node::Node(const Game::Rect& area_, unsigned int entityID_) {
        area = area_;
        entityID = entityID_;
    }

    Game::Rect BehaviourProfile::Node::getRect() const {
        return area;
    }

    const std::vector<Game::Vector> BehaviourProfile::degreesOfMovement = { Game::Vector(1, 0), Game::Vector(-1, 0), Game::Vector(0, 1), Game::Vector(0, -1), Game::Vector(1, 1), Game::Vector(-1, -1), Game::Vector(1, -1), Game::Vector(-1, 1) };

    bool BehaviourProfile::Node::entityValid() const {
        return map && map->getEntityWithID(entityID);
    }

    bool BehaviourProfile::Node::operator==(const Node& node) const {
        return area == node.getRect();
    }

    std::vector<BehaviourProfile::Node> BehaviourProfile::Node::getAdjacent() const {
        std::vector<Node> adjacent;
        for (Game::Vector movement : degreesOfMovement) {
            Game::Rect moveRect = Game::Rect(Game::Vector(area.topLeft.x + movement.x, area.topLeft.y + movement.y), area.width, area.height);
            if (entityValid() && map->entityCanMoveToSpace(entityID, moveRect)) {
                adjacent.push_back(Node(moveRect, entityID));
            }
        }
        return adjacent;
    }

    std::queue<BehaviourProfile::Node> BehaviourProfile::reverseQueue(const std::queue<BehaviourProfile::Node>& reversing) {
        std::vector<Node> tempVec;
        std::queue<Node> copyQueue = reversing;
        while (!copyQueue.empty()) {
            tempVec.push_back(copyQueue.front());
            copyQueue.pop();
        }
        std::reverse(tempVec.begin(), tempVec.end());
        for (BehaviourProfile::Node node : tempVec) {
            copyQueue.push(node);
        }
        return copyQueue;
    }

    std::queue<BehaviourProfile::Node> BehaviourProfile::getPath(const Game::Vector& target) {
        if (!entityValid()) {
            return std::queue<BehaviourProfile::Node>();
        }
        std::queue<Node> path;
        Node currentNode = Node(map->getEntityWithID(entityID)->getHitbox(), entityID);

        while(true) {
            if (currentNode.getRect().contains(target)) {
                return path;
            }
            if (!path.empty()) {
                path.push(currentNode);
            }
            if (path.size() > 50) {
                return std::queue<Node>();
            }

            Node closestNode = currentNode;
            for (Node node : currentNode.getAdjacent()) {
                if (manhattanDistance(node.getRect().topLeft, target) < manhattanDistance(closestNode.getRect().topLeft, target)) {
                    closestNode = node;
                }
            }
            currentNode = closestNode;
        }

    }

    void BehaviourProfile::traversePath() {
        if (entityValid()) {
            Game::Rect nextSpot = currentPath.front().getRect();
            Game::Vector movement = Game::Vector(nextSpot.topLeft.x - map->getEntityWithID(entityID)->getHitbox().topLeft.x, nextSpot.topLeft.y - map->getEntityWithID(entityID)->getHitbox().topLeft.y);
            map->getEntityWithID(entityID)->move(movement);
            if (map->getEntityWithID(entityID)->getHitbox() == nextSpot) {
                currentPath.pop();
            }
        }
    }

    void BehaviourProfile::checkIfNeedRepath() {
        if (ticksSinceRepath < repathDelay) {
            ticksSinceRepath++;
        }
        else if (entityValid()) {
            ticksSinceRepath = 0;
            currentPath = getPath(map->getEntityWithID(map->getPlayerID())->getHitbox().getCenter());
        }
    }

    void BehaviourProfile::spawnDamageAction() {
        std::unique_ptr<Targeting> rectTargeting(new RectTargeting(map->getEntityWithID(entityID)->getHitbox()));
        map->addActionToQueue(std::unique_ptr<HitAction>(new HitAction(map->getEntityWithID(entityID)->getFinalStats().stats[EntityStats::STAT::DMG], map, std::move(rectTargeting), &EnemyTeam::ENEMY_TEAM)));
    }

    unsigned int GruntBehaviourProfile::getEntityID() {
        return entityID;
    }

    void GruntBehaviourProfile::tick() {
        checkIfNeedRepath();
        traversePath();
        spawnDamageAction();
    }

    Buff::Buff() {
        changes = EntityStats();
        framesLeft = 0;
        framesMax = 0;
        frameInterval = 0;
    }

    Buff::Buff(const Buff& copying) {
        changes = copying.changes;
        framesLeft = copying.framesLeft;
        framesMax = copying.framesMax;
        frameInterval = copying.frameInterval;
    }

    Buff::Buff(const EntityStats& changes_, unsigned int framesMax_, unsigned int frameInterval_) {
        changes = changes_;
        framesMax = framesMax_;
        frameInterval = frameInterval_;
    }

    unsigned int Buff::getFramesLeft() const {
        return framesLeft;
    }

    unsigned int Buff::getMaxFrames() const {
        return framesMax;
    }

    void Buff::apply(EntityStats& stats) const {
        stats += changes;
    }

    void Buff::tick() {
        if (framesLeft > 0) {
            framesLeft -= 1;
        }
    }

    EntityTemplate::EntityTemplate() {
        stats = EntityStats();
        hitbox = Rect();
    }

    EntityTemplate::EntityTemplate(const EntityStats& stats_, const Rect& hitbox_, BehaviourProfile* behaviourProfile_, Team::TEAM team_) {
        stats = stats_;
        hitbox = hitbox_;
        behaviourProfile = behaviourProfile_;
        team = team_;
    }

    EntityTemplate::EntityTemplate(const EntityTemplate& copying) {
        stats = copying.stats;
        hitbox = copying.hitbox;
        behaviourProfile = copying.behaviourProfile;
    }

    NoTargeting::NoTargeting() {

    }

    std::vector<unsigned int> NoTargeting::isInRange(const std::vector<unsigned int>& entities, Map* map) {
        return std::vector<unsigned int>();
    }

    AllTargeting::AllTargeting() {

    }

    std::vector<unsigned int> AllTargeting::isInRange(const std::vector<unsigned int>& entities, Map* map) {
        return entities;
    }

    RectTargeting::RectTargeting(const Rect& rect_) {
        rect = rect_;
    }

    std::vector<unsigned int> RectTargeting::isInRange(const std::vector<unsigned int>& entities, Map* map) {
        std::vector<unsigned int> entitiesInRange;
        for (unsigned int entityID : entities) {
            if (map && map->getEntityWithID(entityID) && map->getEntityWithID(entityID)->getHitbox().intersects(rect)) {
                entitiesInRange.push_back(entityID);
            }
        }
        return entitiesInRange;
    }

    CircleTargeting::CircleTargeting(const Circle& circle_) {
        circle = circle_;
    }

    std::vector<unsigned int> CircleTargeting::isInRange(const std::vector<unsigned int>& entities, Map* map) {
        std::vector<unsigned int> entitiesInRange;
        for (unsigned int entityID : entities) {
            if (map && map->getEntityWithID(entityID) && circle.intersects(map->getEntityWithID(entityID)->getHitbox())) {
                entitiesInRange.push_back(entityID);
            }
        }
        return entitiesInRange;
    }

    bool Team::validEntity(unsigned int entityID, Map* map) const {
        return map && map->getEntityWithID(entityID);
    }

    std::vector<unsigned int> PlayerTeam::canBeHit(const std::vector<unsigned int>& entities, Map* map) const {
        std::vector<unsigned int> canBeHitEntities;
        for (unsigned int entityID : entities) {
            if (validEntity(entityID, map) && map->getEntityWithID(entityID)->getTeam() == Team::TEAM::ENEMY) {
                canBeHitEntities.push_back(entityID);
            }
        }
        return canBeHitEntities;
    }

    std::vector<unsigned int> PlayerTeam::canBeHealed(const std::vector<unsigned int>& entities, Map* map) const {
        std::vector<unsigned int> canBeHealedEntities;
        for (unsigned int entityID : entities) {
            if (validEntity(entityID, map) && map->getEntityWithID(entityID)->getTeam() == Team::TEAM::PLAYER) {
                canBeHealedEntities.push_back(entityID);
            }
        }
        return canBeHealedEntities;
    }

    std::vector<unsigned int> PlayerTeam::canBeDisplaced(const std::vector<unsigned int>& entities, Map* map) const {
        std::vector<unsigned int> canBeDisplacedEntities;
        for (unsigned int entityID : entities) {
            if (validEntity(entityID, map) && map->getEntityWithID(entityID)->getTeam() == Team::TEAM::ENEMY) {
                canBeDisplacedEntities.push_back(entityID);
            }
        }
        return canBeDisplacedEntities;
    }

    std::vector<unsigned int> EnemyTeam::canBeHit(const std::vector<unsigned int>& entities, Map* map) const {
        std::vector<unsigned int> canBeHitEntities;
        for (unsigned int entityID : entities) {
            if (validEntity(entityID, map) && map->getEntityWithID(entityID)->getTeam() == Team::TEAM::PLAYER) {
                canBeHitEntities.push_back(entityID);
            }
        }
        return canBeHitEntities;
    }

    std::vector<unsigned int> EnemyTeam::canBeHealed(const std::vector<unsigned int>& entities, Map* map) const {
        std::vector<unsigned int> canBeHealedEntities;
        for (unsigned int entityID : entities) {
            if (validEntity(entityID, map) && map->getEntityWithID(entityID)->getTeam() == Team::TEAM::ENEMY) {
                canBeHealedEntities.push_back(entityID);
            }
        }
        return canBeHealedEntities;
    }

    std::vector<unsigned int> EnemyTeam::canBeDisplaced(const std::vector<unsigned int>& entities, Map* map) const {
        std::vector<unsigned int> canBeDisplacedEntities;
        for (unsigned int entityID : entities) {
            if (validEntity(entityID, map) && map->getEntityWithID(entityID)->getTeam() == Team::TEAM::PLAYER) {
                canBeDisplacedEntities.push_back(entityID);
            }
        }
        return canBeDisplacedEntities;
    }

    std::vector<unsigned int> TerrainTeam::canBeHit(const std::vector<unsigned int>& entities, Map* map) const {
        return entities;
    }

    std::vector<unsigned int> TerrainTeam::canBeHealed(const std::vector<unsigned int>& entities, Map* map) const {
        return std::vector<unsigned int>();
    }

    std::vector<unsigned int> TerrainTeam::canBeDisplaced(const std::vector<unsigned int>& entities, Map* map) const {
        return entities;
    }

    const PlayerTeam PlayerTeam::PLAYER_TEAM = PlayerTeam();
    const EnemyTeam EnemyTeam::ENEMY_TEAM = EnemyTeam();
    const TerrainTeam TerrainTeam::TERRAIN_TEAM = TerrainTeam();

    unsigned int Action::getFrameWait() {
        return frameWait;
    }

    unsigned int Action::tick(const std::vector<unsigned int>& entities) {
        if (delayTicks > 0) {
            delayTicks -= 1;
        }
        if (delayTicks == 0) {
            applyAction(entities);
        }
        return delayTicks;
    }

    Action::Action() {
        frameWait = 0;
        delayTicks = 0;
    }

    HitAction::HitAction(unsigned int damage_, Map* ownerMap_, std::unique_ptr<Targeting> targeting_, const Team* teamChecker_) {
        damage = damage_;
        ownerMap = ownerMap_;
        targeting = std::move(targeting_);
        teamChecker = teamChecker_;
    }

    void HitAction::applyAction(const std::vector<unsigned int>& entities) {
        std::vector<unsigned int> targetableEntities = teamChecker->canBeHit(targeting->isInRange(entities, ownerMap), ownerMap);
        for (unsigned int entityID : targetableEntities) {
            if (ownerMap && ownerMap->getEntityWithID(entityID)) {
                Entity* entity = ownerMap->getEntityWithID(entityID);
                EntityStats newStats = entity->getBaseStats();
                newStats.stats[EntityStats::STAT::HP] -= damage;
                entity->setStats(newStats);
            }
        }
    }

    HealAction::HealAction(unsigned int healAmount_, Map* ownerMap_, std::unique_ptr<Targeting> targeting_, const Team* teamChecker_) {
        healAmount = healAmount_;
        ownerMap = ownerMap_;
        targeting = std::move(targeting_);
        teamChecker = teamChecker_;
    }

    void HealAction::applyAction(const std::vector<unsigned int>& entities) {
        std::vector<unsigned int> targetableEntities = targeting->isInRange(entities, ownerMap);
        for (unsigned int entityID : targetableEntities) {
            if (ownerMap && ownerMap->getEntityWithID(entityID)) {
                Entity* entity = ownerMap->getEntityWithID(entityID);
                EntityStats newStats = entity->getBaseStats();
                newStats.stats[EntityStats::STAT::HP] += healAmount;
                entity->setStats(newStats);
            }
        }
    }

    DisplacementAction::DisplacementAction(const Vector& displaceBy_, Map* ownerMap_, std::unique_ptr<Targeting> targeting_, const Team* teamChecker_) {
        displaceBy = displaceBy_;
        ownerMap = ownerMap_;
        targeting = std::move(targeting_);
        teamChecker = teamChecker_;
    }

    void DisplacementAction::applyAction(const std::vector<unsigned int>& entities) {
        std::vector<unsigned int> targetableEntities = targeting->isInRange(entities, ownerMap);
        for (unsigned int entityID : targetableEntities) {
            if (ownerMap && ownerMap->getEntityWithID(entityID)) {
                ownerMap->getEntityWithID(entityID)->moveWithoutModifier(displaceBy);
            }
        }
    }

    Entity::Entity(const EntityTemplate& entityTemplate, unsigned int id_, Map* owner) {
        id = id_;
        hitbox = entityTemplate.hitbox;
        baseStats = entityTemplate.stats;
        behaviourProfile = entityTemplate.behaviourProfile;
        ownerMap = owner;
        team = entityTemplate.team;
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
        int newX = hitbox.topLeft.x + moveBy.x * getFinalStats().statModifiers[EntityStats::STAT_MOD::MOVE];
        int newY = hitbox.topLeft.y + moveBy.y * getFinalStats().statModifiers[EntityStats::STAT_MOD::MOVE];
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
        for (Buff& buff : buffs) {
            buff.apply(tempStats);
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

    Team::TEAM Entity::getTeam() {
        return team;
    }

    void Entity::setTeam(Team::TEAM team_) {
        team = team_;
    }

    void Map::addActionToQueue(std::unique_ptr<Action> action) {
        actions.push_back(std::move(action));
    }

    Map::Map() {
        currentMaxID = 0;
        playableArea = Rect(Vector(0, 0), 0, 0);
    }

    void Map::tickAndApplyActions() {
        std::vector<std::vector<std::unique_ptr<Action>>::iterator> needsErasing;
        for (std::vector<std::unique_ptr<Action>>::iterator action = actions.begin(); action != actions.end(); action++) {
            std::vector<unsigned int> entityIDs = getActiveEntityIDs();
            if ((*action)->tick(getActiveEntityIDs()) == 0) {
                needsErasing.push_back(action);
            }
        }
        for (std::vector<std::unique_ptr<Action>>::iterator erasing : needsErasing) {
            actions.erase(erasing);
        }

        std::vector<std::vector<std::unique_ptr<Entity>>::iterator> deadEntities;
        for (std::vector<std::unique_ptr<Entity>>::iterator entity = entities.begin(); entity != entities.end(); entity++) {
            if ((*entity)->getFinalStats().stats[EntityStats::STAT::HP] < 1) {
                deadEntities.push_back(entity);
            }
        }
        for (std::vector<std::unique_ptr<Entity>>::iterator deadEntity : deadEntities) {
            entities.erase(deadEntity);
        }
    }

    Entity* Map::getEntityWithID(unsigned int ID) {
        for (std::unique_ptr<Entity>& currentEntity : entities) {
            if (currentEntity->getID() == ID) {
                return currentEntity.get();
            }
        }
        return NULL;
    }

    unsigned int Map::createEntity(const EntityTemplate& entityTemplate) {
        entities.push_back(std::move(std::unique_ptr<Entity>(new Entity(entityTemplate, currentMaxID, this))));
        currentMaxID += 1;
        return currentMaxID - 1;
    }

    std::vector<unsigned int> Map::getActiveEntityIDs() {
        std::vector<unsigned int> returnVec;
        for (std::unique_ptr<Entity>& currentEntity : entities) {
            returnVec.push_back(currentEntity->getID());
        }
        return returnVec;
    }

    bool Map::spaceEmpty(const Rect& space) {
        bool empty = true;
        for (std::unique_ptr<Entity>& currentEntity : entities) {
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
        for (std::unique_ptr<Entity>& currentEntity : entities) {
            bool spaceNotEmpty = currentEntity->getHitbox().intersects(space) && currentEntity->getID() != entityID;
            if (spaceNotEmpty || !inPlayableArea) {
                moveable = false;
            }
        }
        return moveable;
    }

    unsigned int Map::getPlayerID() {
        return 0;
    }

}
