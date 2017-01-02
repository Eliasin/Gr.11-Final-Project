#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <memory>

namespace Game {
    class Entity;

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
        bool intersects(const Rect& rect) const;
    };

    struct EntityStats {
        EntityStats();
        EntityStats(const EntityStats& copying);
        EntityStats operator+(const EntityStats& adding);
        EntityStats operator-(const EntityStats& subtracting);
        void operator+=(const EntityStats& adding);
        void operator-=(const EntityStats& subtracting);
        int max_health, health, stamina, armor, moveSpeed, sight, attackSpeed;
        int max_health_p, stamina_p, armor_p, moveSpeed_p, sight_p, attackSpeed_p;
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
    public:
        enum class Type {
            STATS,
            MOVEMENT
        };
        union Info {
            Info();
            Info(const Info& info_a, const Type& type);
            EntityStats statChanges;
            MovementMods movementChanges;
        };
        struct BuffTemplate {
            Info info;
            Type type;
            int maxFrames, frameInterval;
        };
    private:
        unsigned int framesLeft;
        unsigned int framesMax;
        unsigned int frameInterval;
        Info info;
        Type type;
    public:
        Buff(const Info& buffInfo, const Type& typeInfo, unsigned int frames, unsigned int interval);
        Buff(const Buff& copying);
        unsigned int getFramesLeft();
        unsigned int getMaxFrames();
        const Info& getInfo();
        Type getType();
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
        virtual std::vector<Entity*>& isInRange(std::vector<Entity*>& entities)=0;
    };

    class NoTargeting : public Targeting {
    public:
        NoTargeting();
        std::vector<Entity*>& isInRange(std::vector<Entity*>& entities);
    };

    class AllTargeting : public Targeting {
    public:
        AllTargeting();
        std::vector<Entity*>& isInRange(std::vector<Entity*>& entities);
    };

    class RectTargeting : public Targeting {
        Rect rect;
    public:
        RectTargeting(const Rect& rect_);
        std::vector<Entity*>& isInRange(std::vector<Entity*>& entities);
    };

    class Action {
    protected:
        unsigned int frameWait;
        Targeting* targeting;
    public:
        void setTargeting(Targeting* targeting);
        unsigned int getFrameWait();
        void tickFrameWait();
        virtual void applyAction(std::vector<Entity*>& entities)=0;
    };

    class HitAction : public Action {
        unsigned int damage;
    public:
        HitAction(unsigned int damage_);
        void applyAction(std::vector<Entity*>& entities);
    };

    class HealAction : public Action {
        unsigned int healAmount;
    public:
        HealAction(unsigned int healAmount_);
        void applyAction(std::vector<Entity*>& entities);
    };

    class DisplacementAction : public Action {
        Vector displaceBy;
        DisplacementAction(const Vector& displaceBy_);
        void applyAction(std::vector<Entity*>& entities);
    };

    class Map {
        unsigned int currentMaxID;
        std::vector<Entity*> entities;
        std::vector<Action*> actions;
    public:
        Map();
        ~Map();
        void addActionToQueue(Action* action);
        void tickAndApplyActions();
        Entity* getEntityWithID(unsigned int ID);
        void createEntity(const EntityTemplate& entityTemplate);
        std::vector<unsigned int> getActiveEntityIDs();
        bool spaceEmpty(const Rect& space);
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
        void setHitbox(Rect new_hitbox);
        void move(Vector move_by);
        void addBuff(const Buff& buff);
        const EntityStats& getBaseStats();
        void setStats(const EntityStats& stats);
        EntityStats getFinalStats();
        EntityTemplate getState();
    };

    class Director {
        unsigned int pointPool;
        Map* handling;
        std::map<EntityTemplate, unsigned int> spawnablesValues;
        std::vector<Rect> spawnArea;
    public:
        Director(Map* handling_);
        void addSpawnableEntity(const EntityTemplate& entity);
        void addSpawnArea(const Rect& area);
        void setPointPool(unsigned int pointPool_);
        unsigned int getPointPool();
        void setHandlingMap(Map* handling_);
        Map* getHandlingMap();
        const std::map<EntityTemplate, unsigned int> getSpawnableValuesMap();
    };

}
