
#include "ecs.h"

/* Entity */
/* CreateEntity() */
/* { */
/*     static std::uint32_t next_id = 0; */
/*     return next_id++; */
/* } */

int component_id_counter = 0;
template<typename T> Component
GetComponentId()
{
    static int component_id = component_id_counter++;
    return component_id;
}
