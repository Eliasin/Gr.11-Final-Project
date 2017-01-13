#include "io.hpp"
#include "gameLogic.hpp"

namespace IO {

    EntityMovementKeyHandler::EntityMovementKeyHandler(const std::map<sf::Keyboard::Key, Game::Vector>& keyMovementMap_, Game::Map* map_, unsigned int entityID_) {
        keyMovementMap = keyMovementMap_;
        map = map_;
        entityID = entityID_;
    }

    bool EntityMovementKeyHandler::entityValid() {
        if (map && map->getEntityWithID(entityID)) {
            return true;
        }
        return false;
    }

    void EntityMovementKeyHandler::checkForKeyPress() {
        for (std::pair<sf::Keyboard::Key, Game::Vector> keyVectorPair : keyMovementMap) {
            if (sf::Keyboard::isKeyPressed(keyVectorPair.first)) {
                onKeyPress(keyVectorPair.first);
            }
        }
    }

    void EntityMovementKeyHandler::onKeyPress(sf::Keyboard::Key pressed) {
        if (entityValid()) {
            map->getEntityWithID(entityID)->move(keyMovementMap[pressed]);
        }
    }

    PlayerAttackMouseHandler::PlayerAttackMouseHandler(unsigned int entityID_, Game::Map* map_, Rendering::Camera* camera_, sf::Window* window_, std::vector<Rendering::Animation>* animations_, std::vector<sf::Texture>* attackAnimation_) {
        entityID = entityID_;
        map = map_;
        camera = camera_;
        framesSinceAttack = 0;
        animations = animations_;
        attackAnimation = attackAnimation_;
        window = window_;
    }

    bool PlayerAttackMouseHandler::entityValid() {
        return map && map->getEntityWithID(entityID);
    }

    void PlayerAttackMouseHandler::spawnAttackAction(Game::Vector pos) {
        int entityRange = map->getEntityWithID(entityID)->getFinalStats().stats[Game::EntityStats::STAT::RNG];
        int entityDamage = map->getEntityWithID(entityID)->getFinalStats().stats[Game::EntityStats::STAT::DMG];

        std::unique_ptr<Game::CircleTargeting> targeting(new Game::CircleTargeting(Game::Circle(pos, entityRange)));
        map->addActionToQueue(std::move(std::unique_ptr<Game::Action>(new Game::HitAction(static_cast<unsigned int>(entityDamage), map, std::move(targeting), &Game::PlayerTeam::PLAYER_TEAM))));

        animations->push_back(Rendering::Animation(attackAnimation, camera, window, Game::Rect(Game::Vector(pos.x + entityRange / 2, pos.y - entityRange / 2), entityRange, entityRange), 10));
    }

    void PlayerAttackMouseHandler::onMouseEvent(sf::Vector2<int> position, sf::Mouse::Button pressed) {
        bool enoughTimeSinceLastAttack = framesSinceAttack >= static_cast<unsigned int>(map->getEntityWithID(entityID)->getFinalStats().stats[Game::EntityStats::STAT::ATK_DELAY]);
        if (entityValid() && camera && enoughTimeSinceLastAttack) {
            framesSinceAttack = 0;

            Game::Vector gameClickPosition = camera->reverseTranslate(position);
            Game::Rect entityHitbox = map->getEntityWithID(entityID)->getHitbox();
            Game::Vector entityPosition = Game::Vector(entityHitbox.topLeft.x - entityHitbox.width, entityHitbox.topLeft.y + entityHitbox.height / 2);
            int entityRange = map->getEntityWithID(entityID)->getFinalStats().stats[Game::EntityStats::STAT::RNG];

            std::vector<Game::Vector> attackPoints;
            Game::Vector startPoint = Game::Vector(entityPosition.x, entityPosition.y - entityRange);

            const unsigned int DIRECTIONS = 16;
            for (unsigned int i = 0; i <= DIRECTIONS - 1; i++) {
                attackPoints.push_back(Game::rotatePoint(startPoint, entityPosition, (360/DIRECTIONS*i)));
            }
            Game::Vector closest = startPoint;
            for (Game::Vector attackPoint : attackPoints) {
                if (manhattanDistance(attackPoint, gameClickPosition) < manhattanDistance(closest, gameClickPosition)) {
                    closest = attackPoint;
                }
            }
            spawnAttackAction(closest);
        }
    }

    void PlayerAttackMouseHandler::checkForMouseEvents() {
        framesSinceAttack++;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            onMouseEvent(sf::Mouse::getPosition(), sf::Mouse::Left);
        }
    }

}
