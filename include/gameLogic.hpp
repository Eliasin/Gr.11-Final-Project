#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <memory>

namespace Game {
    class Entity;
    class Map;

    struct Vector {
        int x,y;
        Vector(int x_a, int y_a);
        Vector(const Vector& vector);
        Vector();
    };

    struct Circle {
        Vector center;
        int radius;
        Circle();
        Circle(const Vector& center, unsigned int radius);
        bool contains(Vector& point);
    };

    struct Rect {
        Vector topLeft;
        int width, height;
        Rect();
        Rect(const Vector& topLeft_a, unsigned int width_a, unsigned int height_a);
        bool contains(const Vector& point) const;
        bool contains(const Rect& rect) const;
        Game::Vector getCenter() const;
        bool intersects(const Rect& rect) const;
    };

    struct EntityStats {
        EntityStats();
        EntityStats(const EntityStats& copying);
        EntityStats operator+(const EntityStats& adding);
        EntityStats operator-(const EntityStats& subtracting);
        void operator=(const EntityStats& copying);
        void operator+=(const EntityStats& adding);
        void operator-=(const EntityStats& subtracting);
        std::map<std::string, int> stats;
        std::map<std::string, float> statModifiers;
    };

    struct MovementMods {
        MovementMods();
        bool ghosting, noclip, flying;
    };

    class BehaviourProfile {
    public:
        BehaviourProfile();
        bool isHitting();
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

    struct EntityTemplate {
        EntityTemplate();
        EntityTemplate(const EntityStats& stats_, const Rect& hitbox_, BehaviourProfile* behaviourProfile_);
        EntityTemplate(const EntityTemplate& copying);
        EntityStats stats;
        Rect hitbox;
        BehaviourProfile* behaviourProfile;
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

    class Action {
    protected:
        unsigned int frameWait;
        Targeting* targeting;
        Map* ownerMap;
    public:
        void setTargeting(Targeting* targeting);
        unsigned int getFrameWait();
        void tickFrameWait();
        virtual void applyAction(std::vector<Entity*>& entities)=0;
    };

    class HitAction : public Action {
        unsigned int damage;
    public:
        HitAction(unsigned int damage_, Map* ownerMap_);
        void applyAction(const std::vector<unsigned int>& entities);
    };

    class HealAction : public Action {
        unsigned int healAmount;
    public:
        HealAction(unsigned int healAmount_, Map* ownerMap_);
        void applyAction(const std::vector<unsigned int>& entities);
    };

    class DisplacementAction : public Action {
        Vector displaceBy;
        DisplacementAction(const Vector& displaceBy_, Map* ownerMap_);
        void applyAction(const std::vector<unsigned int>& entities);
    };

    class Map {
        unsigned int currentMaxID;
        std::vector<Entity*> entities;
        std::vector<Action*> actions;
        Rect playableArea;
    public:
        Map();
        ~Map();
        void addActionToQueue(Action* action);
        void tickAndApplyActions();
        Entity* getEntityWithID(unsigned int ID);
        unsigned int createEntity(const EntityTemplate& entityTemplate);
        std::vector<unsigned int> getActiveEntityIDs();
        bool spaceEmpty(const Rect& space);
        void setPlayableArea(const Rect& playableArea_);
        bool entityCanMoveToSpace(unsigned int entityID, const Rect& space);
        const std::vector<Entity*>& getEntities();
    };

    class Entity {
        friend Map;
        unsigned int id;
        Rect hitbox;
        std::vector<Buff> buffs;
        EntityStats baseStats;
        BehaviourProfile* behaviourProfile;
        Map* ownerMap;
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
    };

}
