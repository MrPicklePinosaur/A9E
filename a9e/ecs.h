#ifndef __A9E_ECS_H__
#define __A9E_ECS_H__

#include <iostream>
#include <chrono>
#include <cstdint>
#include <bitset>
#include <any>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <memory>
#include <queue>
#include <map>

#include "renderer.h"
#include "inputer.h"

const int MAX_COMPONENTS = 32;
const int MAX_ENTITIES = 2048; // if we switch to using vector instead of array we can have this be uncapped

using Entity = std::uint32_t;
using ComponentId = std::uint8_t;
using ComponentSignature = std::bitset<MAX_COMPONENTS>;
using EntityComponent = std::map<Entity, ComponentSignature>;

class ComponentArrayBase;
template<typename T> class ComponentArray;
class System;
class ComponentManager;
class EntityManager;
class SystemManager;
class EntityView;

class Scene
{
    std::unique_ptr<ComponentManager> cm;
    std::unique_ptr<EntityManager> em;
    std::unique_ptr<SystemManager> sm;
    Renderer* renderer;
    Inputer* inputer;
    std::any global; // user can use for whatever purpose

    float time_step = 0.05f;

    // for use of system
    std::unordered_set<Entity> kill_list; // set prevents duplicate deletes
    float delta = 0.0f;
    bool terminate_scene = false;
public:
    Scene(Renderer* renderer, Inputer* inputer);
    ~Scene();
    void Run();
    Entity CreateEntity();
    void DestroyEntity(Entity e); // request entity be destroyed
    template<typename T> void AddComponent(Entity e, const T& component);
    template<typename T> void RemoveComponent(Entity e);
    template<typename T> T& GetComponent(Entity e);
    template<typename T> bool HasComponent(Entity e);
    template<typename... ComponentIds> EntityView MakeEntityView();
    template<typename T> ComponentId GetComponentId();
    template<typename T> T* RegisterSystem();
    Renderer* GetRenderer() { return renderer; }
    Inputer* GetInputer() { return inputer; }
    float getDelta() { return delta; }
    template<typename T> void setGlobal(const T& g);
    template<typename T> T& getGlobal();
    void TerminateScene() { terminate_scene = true; }
    void Debug();
private:
    void PurgeKillList(); // actually destroys entity
};

class EntityManager
{
    Scene& scene;
    EntityComponent ec;
    std::queue<Entity> free_list;
    Entity next_entity;
public:
    EntityManager(Scene& scene);
    ~EntityManager();
    Entity CreateEntity();
    void DestroyEntity(Entity e);
    template<typename T> void AddComponent(Entity e);
    template<typename T> void RemoveComponent(Entity e);
    template<typename... ComponentIds> EntityView MakeEntityView();
    void Debug();
};

class EntityView
{
    EntityComponent& ec;
    ComponentSignature signature;
public:

    class iterator {
        EntityComponent::iterator it;
        EntityComponent::iterator it_end;
        ComponentSignature signature;
        iterator(EntityComponent::iterator it_beg, EntityComponent::iterator it_end, ComponentSignature signature);
    public:
        bool operator!=(const iterator& other) const;
        const Entity& operator*() const;
        iterator& operator++();
        friend class EntityView;
    };

    EntityView(EntityComponent& ec, ComponentSignature signature);
    iterator begin();
    iterator end();
};

class SystemManager
{
    Scene& scene;
    std::vector<std::unique_ptr<System>> systems;
public:
    SystemManager(Scene& scene);
    ~SystemManager();
    template<typename T> T* RegisterSystem();
    void UpdateAll();
    void Debug();
};

class System
{
protected:
    Scene& scene;
public:
    System(Scene& scene);
    virtual ~System() = default;
    void Update();
private:
    virtual void BeforeUpdate() = 0;
    virtual void OnUpdate() = 0;
    virtual void AfterUpdate() = 0;
};

class ComponentManager
{
    Scene& scene;
    std::unordered_map<ComponentId, std::shared_ptr<ComponentArrayBase>> ca_pool;
public:
    ComponentManager(Scene& scene);
    ~ComponentManager();
    template<typename T> void AddComponent(Entity e, const T& component);
    template<typename T> void RemoveComponent(Entity e);
    void RemoveAllComponents(Entity e);
    template<typename T> T& GetComponent(Entity e);
    template<typename T> bool HasComponent(Entity e);
    template<typename T> ComponentId GetComponentId();
    void Debug();
private:
    template<typename T> std::shared_ptr<ComponentArray<T>> GetComponentArray();
};

class ComponentArrayBase
{
public:
    virtual ~ComponentArrayBase() = default;
    virtual void RemoveComponent(Entity e) = 0;
    virtual bool HasComponent(Entity e) = 0;
};

template<typename T>
class ComponentArray final : public ComponentArrayBase
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
    bool HasComponent(Entity e);

    using iterator = typename std::array<T, MAX_ENTITIES>::iterator;
    inline iterator begin() { return ca.begin(); }
    inline iterator end() { return ca.begin()+ca_size; }
};

/* =-=-=-=-= Scene =-=-=-=-=-= */

template<typename T> void
Scene::AddComponent(Entity e, const T& component)
{
    cm->AddComponent<T>(e, component);
    em->AddComponent<T>(e);
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

template<typename T> bool
Scene::HasComponent(Entity e)
{
    return cm->HasComponent<T>(e);
}

template<typename... ComponentIds> EntityView
Scene::MakeEntityView()
{
    return em->MakeEntityView<ComponentIds...>();
}

template<typename T> ComponentId
Scene::GetComponentId()
{
    return cm->GetComponentId<T>();
}

template<typename T> T*
Scene::RegisterSystem()
{
    return sm->RegisterSystem<T>();
}

template<typename T> void
Scene::setGlobal(const T& g)
{
    global = g;
}

template<typename T> T&
Scene::getGlobal()
{
    return std::any_cast<T&>(global);
}

/* =-=-=-=-= SystemManager =-=-=-=-=-= */

template<typename T> T*
SystemManager::RegisterSystem()
{
    systems.push_back(std::make_unique<T>(scene));
    return dynamic_cast<T*>(systems.back().get());
}

/* =-=-=-=-= EntityManager =-=-=-=-=-= */

template<typename T> void
EntityManager::AddComponent(Entity e)
{
    ComponentId c_id = scene.GetComponentId<T>();

    // modify entity's component signature to include the id
    if (ec.find(e) == ec.end()) throw "Entity does not exist";
    if (ec[e].test(c_id)) throw "Entity already has component";
    ec[e].set(c_id);
}

template<typename T> void
EntityManager::RemoveComponent(Entity e)
{
    ComponentId c_id = scene.GetComponentId<T>();

    // modify entity's component signature to uninclude the id
    if (ec.find(e) == ec.end()) throw "Entity does not exist";
    if (!ec[e].test(c_id)) throw "Entity does not have component";
    ec[e].reset(c_id);
}

template<typename... ComponentIds> EntityView
EntityManager::MakeEntityView()
{
    ComponentSignature signature;
    ComponentId ids[] = { scene.GetComponentId<ComponentIds>() ... };
    for (size_t i = 0; i < sizeof...(ComponentIds); ++i) signature.set(ids[i]);
    return EntityView{ec, signature};
}

/* =-=-=-=-= ComponentManager =-=-=-=-=-= */

template<typename T> void
ComponentManager::AddComponent(Entity e, const T& component)
{
    std::shared_ptr<ComponentArray<T>> ca = GetComponentArray<T>();
    ca->AddComponent(e, component);
}

template<typename T> void
ComponentManager::RemoveComponent(Entity e)
{
    std::shared_ptr<ComponentArray<T>> ca = GetComponentArray<T>();
    ca->RemoveComponent(e);
}

template<typename T> T&
ComponentManager::GetComponent(Entity e)
{
    std::shared_ptr<ComponentArray<T>> ca = GetComponentArray<T>();
    return ca->GetComponent(e);
}

template<typename T> bool
ComponentManager::HasComponent(Entity e)
{
    std::shared_ptr<ComponentArray<T>> ca = GetComponentArray<T>();
    return ca->HasComponent(e);
}

extern int component_id_counter;
template<typename T> ComponentId
ComponentManager::GetComponentId()
{
    static int component_id = component_id_counter++;
    return component_id;
}

template<typename T> std::shared_ptr<ComponentArray<T>>
ComponentManager::GetComponentArray()
{
    ComponentId id = GetComponentId<T>();

    // check if we already have a component array for this (if not, create)
    if (ca_pool.find(id) == ca_pool.end())
        ca_pool.insert(std::pair(id, std::make_shared<ComponentArray<T>>()));

    return std::static_pointer_cast<ComponentArray<T>>(ca_pool[id]);
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
    if (entity_to_index.find(e) != entity_to_index.end())
        throw "Component already added";

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
    entity_to_index[last_entity] = remove_entity_ind;
    index_to_entity[remove_entity_ind] = last_entity;
    entity_to_index.erase(e);
    index_to_entity.erase(last_entity_ind);

    --ca_size;
}

template<typename T> T&
ComponentArray<T>::GetComponent(Entity e)
{
    if (entity_to_index.find(e) == entity_to_index.end())
        throw "Entity does not have component";

    return ca[entity_to_index.at(e)];
}

template<typename T> bool
ComponentArray<T>::HasComponent(Entity e)
{
    return (entity_to_index.find(e) != entity_to_index.end());
}

#endif // __A9E_ECS_H__
