#pragma once
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <cmath>
#include <map>
#include <unordered_map>
#include <memory>

namespace Game {
    const float PI = 3.14159265359;

    class Entity;
    class Map;

    struct Vector {
        int x,y;
        Vector(int x_a, int y_a);
        Vector(const Vector& vector);
        Vector();
    };

    Vector rotatePoint(Vector point, Vector anchor, float angle);

    float manhattanDistance(Game::Vector p1, Game::Vector p2);

    struct Rect {
        Vector topLeft;
        int width, height;
        Rect();
        Rect(const Vector& topLeft_a, unsigned int width_a, unsigned int height_a);
        bool contains(const Vector& point) const;
        bool contains(const Rect& rect) const;
        Game::Vector getCenter() const;
        bool intersects(const Rect& rect) const;
        bool operator==(const Rect& rect) const;
    };

    struct Circle {
        Vector center;
        int radius;
        Circle();
        Circle(const Vector& center, unsigned int radius);
        bool intersects(const Rect& rect) const;
        bool contains(const Vector& point) const;
        bool contains(const Rect& rect) const;
    };

    struct EntityStats {
        enum class STAT {
            MAX_HP,
            HP,
            MAX_STAM,
            STAM,
            SIGHT,
            ATK_DELAY,
            RNG,
            DMG
        };
        enum class STAT_MOD {
            MAX_HP,
            MAX_STAM,
            SIGHT,
            ATK_DELAY,
            MOVE,
            DMG
        };
        EntityStats();
        EntityStats(const EntityStats& copying);
        EntityStats operator+(const EntityStats& adding);
        EntityStats operator-(const EntityStats& subtracting);
        void operator=(const EntityStats& copying);
        void operator+=(const EntityStats& adding);
        void operator-=(const EntityStats& subtracting);
        std::map<STAT, int> stats;
        std::map<STAT_MOD, float> statModifiers;
    };

    class BehaviourProfile {
    protected:
        static const int repathDelay = 10;
        static const std::vector<Game::Vector> degreesOfMovement;

        class Node {
            Game::Rect area;
            unsigned int entityID;
            Game::Map* map;
            bool entityValid() const;
        public:
            Node(const Game::Rect& area_, unsigned int entityID_);
            std::vector<Node> getAdjacent() const;
            Game::Rect getRect() const;
            bool operator==(const Node& node) const;
            bool operator<(const Node& node) const;
        };

        std::queue<Node> currentPath;
        std::queue<Node> reverseQueue(const std::queue<Node>& reversing);
        std::queue<Node> getPath(const Game::Vector& target);

        unsigned int ticksSinceRepath;
        unsigned int entityID;
        Map* map;
        virtual void traversePath();
        virtual void checkIfNeedRepath();
        virtual void spawnDamageAction();
    public:
        virtual unsigned int getEntityID()=0;
        virtual bool entityValid() const;
        virtual void tick()=0;
    };

    class GruntBehaviourProfile : public BehaviourProfile {
    protected:

    public:
        GruntBehaviourProfile(unsigned int entityID, Map* map);
        virtual unsigned int getEntityID() override;
        virtual void tick() override;
    };

    class Buff {
    protected:
        EntityStats changes;
        unsigned int framesLeft;
        unsigned int framesMax;
        unsigned int frameInterval;
    public:
        Buff();
        Buff(const Buff& copying);
        Buff(const EntityStats& changes_, unsigned int framesMax_, unsigned int frameInterval_);
        unsigned int getFramesLeft() const;
        unsigned int getMaxFrames() const;
        void apply(EntityStats& stats) const;
        void tick();
    };

    class Targeting {
    public:
        virtual std::vector<unsigned int> isInRange(const std::vector<unsigned int>& entities, Map* map)=0;
    };

    class NoTargeting : public Targeting {
    public:
        NoTargeting();
        std::vector<unsigned int> isInRange(const std::vector<unsigned int>& entities, Map* map) override;
    };

    class AllTargeting : public Targeting {
    public:
        AllTargeting();
        std::vector<unsigned int> isInRange(const std::vector<unsigned int>& entities, Map* map) override;
    };

    class RectTargeting : public Targeting {
        Rect rect;
    public:
        RectTargeting(const Rect& rect_);
        std::vector<unsigned int> isInRange(const std::vector<unsigned int>& entities, Map* map) override;
    };

    class CircleTargeting : public Targeting {
        Circle circle;
    public:
        CircleTargeting(const Circle& circle_);
        std::vector<unsigned int> isInRange(const std::vector<unsigned int>& entities, Map* map) override;
    };

    class Team {
    protected:
        bool validEntity(unsigned int entityID, Map* map) const;
    public:
        enum class TEAM {
            PLAYER,
            ENEMY,
            TERRAIN
        };
        virtual std::vector<unsigned int> canBeHit(const std::vector<unsigned int>& entities, Map* map) const =0;
        virtual std::vector<unsigned int> canBeHealed(const std::vector<unsigned int>& entities, Map* map) const=0;
        virtual std::vector<unsigned int> canBeDisplaced(const std::vector<unsigned int>& entities, Map* map) const =0;
    };

    class PlayerTeam : public Team {
    public:
        static const PlayerTeam PLAYER_TEAM;
        virtual std::vector<unsigned int> canBeHit(const std::vector<unsigned int>& entities, Map* map) const override;
        virtual std::vector<unsigned int> canBeHealed(const std::vector<unsigned int>& entities, Map* map) const override;
        virtual std::vector<unsigned int> canBeDisplaced(const std::vector<unsigned int>& entities, Map* map) const override;
    };

    class EnemyTeam : public Team {
    public:
        static const EnemyTeam ENEMY_TEAM;
        virtual std::vector<unsigned int> canBeHit(const std::vector<unsigned int>& entities, Map* map) const override;
        virtual std::vector<unsigned int> canBeHealed(const std::vector<unsigned int>& entities, Map* map) const override;
        virtual std::vector<unsigned int> canBeDisplaced(const std::vector<unsigned int>& entities, Map* map) const override;
    };

    class TerrainTeam : public Team {
    public:
        static const TerrainTeam TERRAIN_TEAM;
        virtual std::vector<unsigned int> canBeHit(const std::vector<unsigned int>& entities, Map* map) const override;
        virtual std::vector<unsigned int> canBeHealed(const std::vector<unsigned int>& entities, Map* map) const override;
        virtual std::vector<unsigned int> canBeDisplaced(const std::vector<unsigned int>& entities, Map* map) const override;
    };


    struct EntityTemplate {
        EntityTemplate();
        EntityTemplate(const EntityStats& stats_, const Rect& hitbox_, BehaviourProfile* behaviourProfile_, Team::TEAM team_);
        EntityTemplate(const EntityTemplate& copying);
        EntityStats stats;
        Rect hitbox;
        BehaviourProfile* behaviourProfile;
        Team::TEAM team;
    };

    class Action {
    protected:
        unsigned int frameWait;
        unsigned int delayTicks;
        std::unique_ptr<Targeting> targeting;
        const Team* teamChecker;
        Map* ownerMap;
        virtual void applyAction(const std::vector<unsigned int>& entities)=0;
    public:
        Action();
        void setTargeting(Targeting* targeting);
        unsigned int getFrameWait();
        unsigned int tick(const std::vector<unsigned int>& entities);
    };

    class HitAction : public Action {
        unsigned int damage;
    protected:
        virtual void applyAction(const std::vector<unsigned int>& entities) override;
    public:
        HitAction(unsigned int damage_, Map* ownerMap_, std::unique_ptr<Targeting> targeting_,const Team* teamChecker_);
    };

    class HealAction : public Action {
        unsigned int healAmount;
    protected:
        virtual void applyAction(const std::vector<unsigned int>& entities) override;
    public:
        HealAction(unsigned int healAmount_, Map* ownerMap_, std::unique_ptr<Targeting> targeting_, const Team* teamChecker_);
    };

    class DisplacementAction : public Action {
        Vector displaceBy;
    protected:
        virtual void applyAction(const std::vector<unsigned int>& entities) override;
    public:
        DisplacementAction(const Vector& displaceBy_, Map* ownerMap_, std::unique_ptr<Targeting> targeting_, const Team* teamChecker_);
    };

    class Map {
        unsigned int currentMaxID;
        std::vector<std::unique_ptr<Entity>> entities;
        std::vector<std::unique_ptr<Action>> actions;
        Rect playableArea;
    public:
        Map();
        void addActionToQueue(std::unique_ptr<Action> action);
        void tickAndApplyActions();
        Entity* getEntityWithID(unsigned int ID);
        unsigned int createEntity(const EntityTemplate& entityTemplate);
        std::vector<unsigned int> getActiveEntityIDs();
        bool spaceEmpty(const Rect& space);
        void setPlayableArea(const Rect& playableArea_);
        bool entityCanMoveToSpace(unsigned int entityID, const Rect& space);
        unsigned int getPlayerID();
    };

    class Entity {
        friend Map;
        unsigned int id;
        Rect hitbox;
        std::vector<Buff> buffs;
        EntityStats baseStats;
        BehaviourProfile* behaviourProfile;
        Map* ownerMap;
        Team::TEAM team;
        Entity(const EntityTemplate& entityTemplate, unsigned int id_, Map* owner);
    public:
        Entity(const Entity& entity);
        const unsigned int getID();
        const Rect getHitbox();
        void setHitbox(const Rect& newHitbox);
        void move(const Vector& moveByy);
        void moveWithoutModifier(const Vector& moveBy);
        void addBuff(const Buff& buff);
        const EntityStats& getBaseStats();
        void setStats(const EntityStats& stats);
        EntityStats getFinalStats();
        EntityTemplate getState();
        Team::TEAM getTeam();
        void setTeam(Team::TEAM team_);
    };

}
