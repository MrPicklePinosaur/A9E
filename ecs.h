#ifndef __ECS_H__
#define __ECS_H__

const int MAX_COMPONENTS = 32;
const int MAX_ENTITIES = 1028;

using Entity = std::uint32_t;
using Component = std::uint8_t;
using ComponentSignature = std::bitset<MAX_COMPONENTS>;

// TODO better implementation
/* Entity CreateEntity(); */

template<typename T> Component GetComponentId();


#endif // __ECS_H__
