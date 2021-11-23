#ifndef __ECS_H__
#define __ECS_H__

#include <cstdint>
#include <bitset>
#include <unordered_map>
#include <array>

const int MAX_COMPONENTS = 32;
const int MAX_ENTITIES = 1028;

using Entity = std::uint32_t;
using Component = std::uint8_t;
using ComponentSignature = std::bitset<MAX_COMPONENTS>;

// TODO better implementation
/* Entity CreateEntity(); */


class Scene
{

public:
    Scene();
    ~Scene();
private:
    template<typename T> Component GetComponentId();
};

template<typename T>
class ComponentArray
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

    using iterator = typename std::array<T, MAX_ENTITIES>::iterator;
    inline iterator begin() { return ca.begin(); }
    inline iterator end() { return ca.begin()+ca_size; }
};

int component_id_counter = 0;
template<typename T> Component
GetComponentId()
{
    static int component_id = component_id_counter++;
    return component_id;
}

template<typename T>
ComponentArray<T>::ComponentArray(): ca_size{0} { }

template<typename T>
ComponentArray<T>::~ComponentArray() { }

template<typename T> void
ComponentArray<T>::AddComponent(Entity e, const T& component)
{
    // check if already added?

    if (ca_size == MAX_ENTITIES) {
        throw "Max entities reached. Consider increasing MAX_ENTITIES";
    }

    ca[ca_size] = component;
    entity_to_index[e] = ca_size;
    index_to_entity[ca_size] = e;

    ++ca_size;
}

template<typename T> void
ComponentArray<T>::RemoveComponent(Entity e)
{
    if (entity_to_index.find(e) == entity_to_index.end()) {
        throw "Attempting to remove component that entity does not have";
    }

    size_t remove_entity_ind = entity_to_index[e];
    size_t last_entity_ind = ca_size-1;
    Entity last_entity = index_to_entity[last_entity_ind];

    // move instead?
    ca[remove_entity_ind] = last_entity;

    // update bookkeeping stuff
    entity_to_index.erase(e);
    index_to_entity.erase(last_entity_ind);
    entity_to_index[last_entity] = remove_entity_ind;
    index_to_entity[remove_entity_ind] = last_entity;

    --ca_size;
}

#endif // __ECS_H__
