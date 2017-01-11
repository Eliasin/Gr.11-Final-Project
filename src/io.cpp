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

    bool PlayerAttackMouseHandler::entityValid() {
        return map && map->getEntityWithID(entityID);
    }

    void PlayerAttackMouseHandler::spawnAttackAction(Game::Vector pos) {
        int entityRange = map->getEntityWithID(entityID)->getFinalStats().stats["range"];
        int entityDamage = map->getEntityWithID(entityID)->getFinalStats().stats["damage"];

        std::unique_ptr<Game::CircleTargeting> targeting(new Game::CircleTargeting(Game::Circle(pos, entityRange)));
        Game::HitAction(static_cast<unsigned int>(entityDamage), map, std::move(targeting), &Game::PlayerTeam::PLAYER_TEAM);
    }

    void PlayerAttackMouseHandler::onMouseEvent(sf::Vector2<int> position, sf::Mouse::Button pressed) {
        if (entityValid() && camera) {
            Game::Vector gameClickPosition = camera->reverseTranslate(position);
            Game::Vector entityPosition = map->getEntityWithID(entityID)->getHitbox().getCenter();
            std::pair<Game::Vector, Game::Vector> solutions;
            int entityRange = map->getEntityWithID(entityID)->getFinalStats().stats["range"];

            float simplifiedLineSlope = (float)(gameClickPosition.x - entityPosition.x) / (float)(gameClickPosition.y - entityPosition.y);

            float simpifiedPositiveSolution = (float)entityRange / sqrt(pow(simplifiedLineSlope, 2) + 1);
            float simplifiedNegativeSolution = -1 * (float)entityRange / sqrt(pow(simplifiedLineSlope, 2) + 1);

            solutions.first = Game::Vector(simpifiedPositiveSolution, simpifiedPositiveSolution * simplifiedLineSlope);
            solutions.second = Game::Vector(simplifiedNegativeSolution, simplifiedNegativeSolution * simplifiedLineSlope);

            if (manhattanDistance(solutions.first, gameClickPosition) >= manhattanDistance(solutions.second, gameClickPosition)) {
                spawnAttackAction(solutions.first);
            }
            else {
                spawnAttackAction(solutions.second);
            }
        }
    }

    void PlayerAttackMouseHandler::checkForMouseEvents() {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            onMouseEvent(sf::Mouse::getPosition(), sf::Mouse::Left);
        }
    }

}
