#ifndef __ECS_H__
#define __ECS_H__

#include <iostream>
#include <chrono>
#include <cstdint>
#include <bitset>
#include <unordered_map>
#include <array>
#include <memory>
#include <queue>
#include <map>

#include "renderer.h"
#include "inputer.h"
#include "config.h"

const int MAX_COMPONENTS = 32;
const int MAX_ENTITIES = 1028; // if we switch to using vector instead of array we can have this be uncapped

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
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Inputer> inputer;
    float delta = 0.0f;
public:
    Scene();
    ~Scene();
    void Run();
    Entity CreateEntity();
    void DestroyEntity(Entity e);
    template<typename T> void AddComponent(Entity e, const T& component);
    template<typename T> void RemoveComponent(Entity e);
    template<typename T> T& GetComponent(Entity e);
    template<typename T> bool HasComponent(Entity e);
    template<typename... ComponentIds> EntityView MakeEntityView();
    template<typename T> ComponentId GetComponentId();
    template<typename T> T* RegisterSystem();
    Renderer* GetRenderer() { return renderer.get(); }
    Inputer* GetInputer() { return inputer.get(); }
    float getDelta() { return delta; }
    void Debug();
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
    std::unordered_map<ComponentId, std::unique_ptr<ComponentArrayBase>> ca_pool;
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
    template<typename T> ComponentArray<T>* GetComponentArray();
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

Scene::Scene():
    cm{std::make_unique<ComponentManager>(*this)},
    em{std::make_unique<EntityManager>(*this)},
    sm{std::make_unique<SystemManager>(*this)},
    renderer{std::make_unique<CursesRenderer>()},
    inputer{std::make_unique<CursesInputer>()} { }

Scene::~Scene() {}

void
Scene::Run()
{
    while(true) {
        std::chrono::steady_clock::time_point beg_tick = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(TIME_STEP*1000.0)));

        sm->UpdateAll();
        inputer->ClearKeyMap();

        std::chrono::steady_clock::time_point end_tick = std::chrono::steady_clock::now();
        delta = std::chrono::duration_cast<std::chrono::nanoseconds>(end_tick-beg_tick).count()/1000000000.0f;
    }
}

Entity
Scene::CreateEntity()
{
    return em->CreateEntity();
}

void
Scene::DestroyEntity(Entity e)
{
    // TODO remove all of entity's components
    cm->RemoveAllComponents(e);
    em->DestroyEntity(e);
}

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

void
Scene::Debug()
{
    cm->Debug();
    em->Debug();
    sm->Debug();
}

/* =-=-=-=-= SystemManager =-=-=-=-=-= */

SystemManager::SystemManager(Scene& scene): scene{scene} {}
SystemManager::~SystemManager() {}

template<typename T> T*
SystemManager::RegisterSystem()
{
    systems.push_back(std::make_unique<T>(scene));
    return dynamic_cast<T*>(systems.back().get());
}

void
SystemManager::UpdateAll()
{
    for (auto& s : systems) s->Update();
}

void SystemManager::Debug() {}

/* =-=-=-=-= System =-=-=-=-=-= */

System::System(Scene& scene): scene{scene} {}

void
System::Update()
{
    BeforeUpdate();
    OnUpdate();
    AfterUpdate();
}

/* =-=-=-=-= EntityView::iterator =-=-=-=-=-= */

EntityView::iterator::iterator(EntityComponent::iterator it_beg, EntityComponent::iterator it_end, ComponentSignature signature):
    it{it_beg}, it_end{it_end}, signature{signature}
{
    // find first valid entity
    while (signature != (signature & (*it).second) && it != it_end) ++it;
}

bool
EntityView::iterator::operator!=(const iterator& other) const
{
    if (signature != other.signature) { return false; }
    return it != other.it;
}

const Entity&
EntityView::iterator::operator*() const
{
    return (*it).first;
}

EntityView::iterator&
EntityView::iterator::operator++()
{
    // keep advancing until we find a matching signature
    do { ++it; } while (signature != (signature & (*it).second) && it != it_end);
    return *this;
}

/* =-=-=-=-= EntityView =-=-=-=-=-= */

EntityView::EntityView(EntityComponent& ec, ComponentSignature signature):
    ec{ec}, signature{signature} {}

EntityView::iterator
EntityView::begin()
{
    return iterator{ec.begin(), ec.end(), signature};
}

EntityView::iterator
EntityView::end()
{
    return iterator{ec.end(), ec.end(), signature};
}

/* =-=-=-=-= EntityManager =-=-=-=-=-= */

EntityManager::EntityManager(Scene& scene): scene{scene}, next_entity{0} {}
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

template<> EntityView
EntityManager::MakeEntityView()
{
    /* if no component ids are passed in, assume we want a view for all entities */
    ComponentSignature signature;
    /* signature.set(); // set all bits to 1 */
    return EntityView{ec, signature};
}

template<typename... ComponentIds> EntityView
EntityManager::MakeEntityView()
{
    ComponentSignature signature;
    ComponentId ids[] = { scene.GetComponentId<ComponentIds>() ... };
    for (size_t i = 0; i < sizeof...(ComponentIds); ++i) signature.set(ids[i]);
    return EntityView{ec, signature};
}

void
EntityManager::Debug()
{
    std::cout << "ENTITY MANAGER =-=-=-=-=-=-=" << std::endl;
    for (auto& kv : ec)
        std::cout << kv.second << ":" << kv.first << std::endl;
}

/* =-=-=-=-= ComponentManager =-=-=-=-=-= */

ComponentManager::ComponentManager(Scene& scene): scene{scene} {}
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

void
ComponentManager::RemoveAllComponents(Entity e)
{
    // this is not the cleanest
    for (auto& vt : ca_pool)
        if (vt.second->HasComponent(e)) vt.second->RemoveComponent(e);    
}

template<typename T> T&
ComponentManager::GetComponent(Entity e)
{
    ComponentArray<T>* ca = GetComponentArray<T>();
    return ca->GetComponent(e);
}

template<typename T> bool
ComponentManager::HasComponent(Entity e)
{
    ComponentArray<T>* ca = GetComponentArray<T>();
    return ca->HasComponent(e);
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

void
ComponentManager::Debug()
{
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

template<typename T> bool
ComponentArray<T>::HasComponent(Entity e)
{
    return (entity_to_index.find(e) != entity_to_index.end());
}

#endif // __ECS_H__
