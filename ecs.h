#ifndef __ECS_H__
#define __ECS_H__

#include <cstdint>
#include <bitset>
#include <unordered_map>
#include <array>
#include <memory>
#include <queue>
#include <map>

const int MAX_COMPONENTS = 32;
const int MAX_ENTITIES = 1028; // if we switch to using vector instead of array we can have this be uncapped

using Entity = std::uint32_t;
using ComponentId = std::uint8_t;
using ComponentSignature = std::bitset<MAX_COMPONENTS>;

class ComponentArrayBase;
template<typename T> class ComponentArray;
class System;
class ComponentManager;
class EntityManager;
class SystemManager;

class Scene
{
    std::unique_ptr<ComponentManager> cm;
    std::unique_ptr<EntityManager> em;
    std::unique_ptr<SystemManager> sm;
public:
    Scene();
    ~Scene();
    Entity CreateEntity();
    void DestroyEntity(Entity e);
    template<typename T> void AddComponent(Entity e, const T& component);
    template<typename T> void RemoveComponent(Entity e);
    template<typename T> T& GetComponent(Entity e);
};

class EntityManager
{
    using EntityComponent = std::map<Entity, ComponentSignature>;
    EntityComponent ec;
    std::queue<Entity> free_list;
    Entity next_entity;
public:

#if 0
    template<typename... ComponentIds> 
    class EntityView
    {
    public:
        class iterator {
            EntityComponent::iterator it;
            EntityComponent::iterator it_end; // TODO find a way to get rid of this
            ComponentSignature signature;
            EntityView(EntityComponent::iterator it, EntityComponent::iterator it_end);
        public:
            bool operator!=(const EntityView& other) const;
            const Entity& operator*() const;
            EntityView& operator++();
            friend class EntityView;
        };

        iterator begin();
        iterator end();
    };
#endif

    EntityManager();
    ~EntityManager();
    Entity CreateEntity();
    void DestroyEntity(Entity e);
};


class SystemManager
{
    std::vector<System> systems;
public:
    SystemManager();
    ~SystemManager();
};

class System
{
public:
    System();
    virtual ~System() = default;
    void doUpdate();
private:
    virtual void OnUpdate() = 0;
};

class RendererSystem : public System
{
public:
    RendererSystem();
    ~RendererSystem();
    void OnUpdate();
};

class ComponentManager
{
    std::unordered_map<ComponentId, std::unique_ptr<ComponentArrayBase>> ca_pool;
public:
    ComponentManager();
    ~ComponentManager();
    template<typename T> void AddComponent(Entity e, const T& component);
    template<typename T> void RemoveComponent(Entity e);
    template<typename T> T& GetComponent(Entity e);
    template<typename T> ComponentId GetComponentId();
private:
    template<typename T> ComponentArray<T>* GetComponentArray();
};

class ComponentArrayBase
{
public:
    virtual ~ComponentArrayBase() = default;
};

template<typename T>
class ComponentArray : public ComponentArrayBase
{
    std::array<T, MAX_ENTITIES> ca;
    size_t ca_size;
    std::unordered_map<Entity, size_t> entity_to_index;
    std::unordered_map<size_t, Entity> index_to_entity;
public:
    ComponentArray();
    ~ComponentArray();
    void AddComponent(Entity e, const T& component);
    void RemoveComponent(Entity e);
    T& GetComponent(Entity e);

    using iterator = typename std::array<T, MAX_ENTITIES>::iterator;
    inline iterator begin() { return ca.begin(); }
    inline iterator end() { return ca.begin()+ca_size; }
};

/* =-=-=-=-= Scene =-=-=-=-=-= */

Scene::Scene():
    cm{std::make_unique<ComponentManager>()},
    em{std::make_unique<EntityManager>()},
    sm{std::make_unique<SystemManager>()} {}

Scene::~Scene() {}

Entity
Scene::CreateEntity()
{
    return em->CreateEntity();
}

void
Scene::DestroyEntity(Entity e)
{
    em->DestroyEntity(e);
    // TODO remove all of entity's components too
}

template<typename T> void
Scene::AddComponent(Entity e, const T& component)
{
    cm->AddComponent<T>(e, component);
}

template<typename T> void
Scene::RemoveComponent(Entity e)
{
    cm->RemoveComponent<T>(e);
}

template<typename T> T&
Scene::GetComponent(Entity e)
{
    return cm->GetComponent<T>(e);
}

/* =-=-=-=-= SystemManager =-=-=-=-=-= */

SystemManager::SystemManager() {}
SystemManager::~SystemManager() {}

/* =-=-=-=-= System =-=-=-=-=-= */

void
System::doUpdate()
{
    OnUpdate();
}

/* =-=-=-=-= EntityManager::EntityView =-=-=-=-=-= */

#if 0
template<typename... ComponentIds>
EntityManager::EntityView::EntityView(EntityComponent::iterator it, EntityComponent::iterator it_end):
    it{it}, it_end{it_end}, signature{0}
{
    /* if no component ids are passed in, assume we want a view for all entities */
    if (sizeof...(ComponentIds) == 0) {
        signature.set(); // set all bits to 1
    } else {
        // TODO need reference to component manager for this
        /* ComponentIds ids[] = { GetComponentId<ComponentIds>()... }; */
        /* for (int i = 0; i < sizeof...(ComponentIds); ++i) signature.set(ids[i]); */
    }
}

bool
EntityManager::EntityView::operator!=(const EntityView& other) const
{
    if (signature != other.signature) return false;
    return it == other.it;
}

const Entity&
EntityManager::EntityView::operator*() const
{
    return (*it).first;
}

EntityManager::EntityView&
EntityManager::EntityView::operator++()
{
    // keep advancing until we find a matching signature
    do { ++it; } while (signature != (signature & (*it).second) && it != it_end);
    return *this;
}
#endif

/* =-=-=-=-= EntityManager =-=-=-=-=-= */

EntityManager::EntityManager(): next_entity{0} {}
EntityManager::~EntityManager() {}

// could add overload to 'preload' entity with components
Entity
EntityManager::CreateEntity()
{
    // check if we are at max entities
    if (next_entity == MAX_ENTITIES && free_list.empty())
        throw "Max entity limit reached, consider increasing MAX_ENTITIES";

    Entity new_entity;
    if (free_list.empty()) {
        // if no items in free list, assign next id
        new_entity = next_entity++;
    } else {
        // otherwise we can take from free list
        new_entity = free_list.front();
        free_list.pop();
    }

    ec[new_entity];

    return new_entity;
}

void 
EntityManager::DestroyEntity(Entity e)
{
    size_t erased = ec.erase(e);
    if (erased == 0) throw "Attempted to destroy non-existant entity";

    free_list.push(e);
}

#if 0
template<typename... ComponentIds> EntityManager::EntityView
EntityManager::begin()
{
    return EntityView<ComponentIds...>(ec.begin(), ec.end());
}

template<typename... ComponentIds> EntityManager::EntityView
EntityManager::end()
{
    return EntityView<ComponentIds...>(ec.end(), ec.end());
}
#endif

/* =-=-=-=-= ComponentManager =-=-=-=-=-= */

ComponentManager::ComponentManager() {}
ComponentManager::~ComponentManager() {}

template<typename T> void
ComponentManager::AddComponent(Entity e, const T& component)
{
    ComponentArray<T>* ca = GetComponentArray<T>();
    ca->AddComponent(e, component);
}

template<typename T> void
ComponentManager::RemoveComponent(Entity e)
{
    ComponentArray<T>* ca = GetComponentArray<T>();
    ca->RemoveComponent(e);
}

template<typename T> T&
ComponentManager::GetComponent(Entity e)
{
    ComponentArray<T>* ca = GetComponentArray<T>();
    return ca->GetComponent(e);
}

int component_id_counter = 0;
template<typename T> ComponentId
ComponentManager::GetComponentId()
{
    static int component_id = component_id_counter++;
    return component_id;
}

template<typename T> ComponentArray<T>*
ComponentManager::GetComponentArray()
{
    ComponentId id = GetComponentId<T>();

    // check if we already have a component array for this (if not, create)
    if (ca_pool.find(id) == ca_pool.end())
        ca_pool[id] = std::make_unique<ComponentArray<T>>();

    return static_cast<ComponentArray<T>*>(ca_pool[id].get());
}

/* =-=-=-=-= ComponentArray =-=-=-=-=-= */

template<typename T>
ComponentArray<T>::ComponentArray(): ca_size{0} { }

template<typename T>
ComponentArray<T>::~ComponentArray() { }

template<typename T> void
ComponentArray<T>::AddComponent(Entity e, const T& component)
{
    // check if already added?

    if (ca_size == MAX_ENTITIES)
        throw "Max entities reached. Consider increasing MAX_ENTITIES";

    ca[ca_size] = component;
    entity_to_index[e] = ca_size;
    index_to_entity[ca_size] = e;

    ++ca_size;
}

template<typename T> void
ComponentArray<T>::RemoveComponent(Entity e)
{
    if (entity_to_index.find(e) == entity_to_index.end())
        throw "Attempting to remove component that entity does not have";

    size_t remove_entity_ind = entity_to_index[e];
    size_t last_entity_ind = ca_size-1;
    Entity last_entity = index_to_entity[last_entity_ind];

    // move instead?
    ca[remove_entity_ind] = ca[last_entity_ind];

    // update bookkeeping stuff
    entity_to_index.erase(e);
    index_to_entity.erase(last_entity_ind);
    entity_to_index[last_entity] = remove_entity_ind;
    index_to_entity[remove_entity_ind] = last_entity;

    --ca_size;
}

template<typename T> T&
ComponentArray<T>::GetComponent(Entity e)
{
    if (entity_to_index.find(e) == entity_to_index.end())
        throw "Entity does not have component";

    return ca[entity_to_index.at(e)];
}

#endif // __ECS_H__
