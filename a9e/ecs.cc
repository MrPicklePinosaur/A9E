
#include "ecs.h"

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

        PurgeKillList();

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
    kill_list.insert(e);
}

void
Scene::Debug()
{
    cm->Debug();
    em->Debug();
    sm->Debug();
}

void
Scene::PurgeKillList()
{
    for (auto& e: kill_list) {
        cm->RemoveAllComponents(e);
        em->DestroyEntity(e);
    }
    kill_list.clear();
}

/* =-=-=-=-= SystemManager =-=-=-=-=-= */

SystemManager::SystemManager(Scene& scene): scene{scene} {}
SystemManager::~SystemManager() {}

void SystemManager::Debug() {}

void
SystemManager::UpdateAll()
{
    for (auto& s : systems) s->Update();
}

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

int component_id_counter = 0;

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

template<> EntityView
EntityManager::MakeEntityView()
{
    /* if no component ids are passed in, assume we want a view for all entities */
    ComponentSignature signature;
    /* signature.set(); // set all bits to 1 */
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

void
ComponentManager::RemoveAllComponents(Entity e)
{
    // this is not the cleanest
    for (auto& vt : ca_pool)
        if (vt.second.get()->HasComponent(e)) {
            vt.second.get()->RemoveComponent(e);    
        }
}

void
ComponentManager::Debug()
{
}
