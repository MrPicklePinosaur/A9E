#ifndef __TRANSFORM_COMPONENT_H__
#define __TRANSFORM_COMPONENT_H__

#include "../math/vec.h"

struct Transform {
    vec2 pos;
    int layer = 0;
};

#endif // __TRANSFORM_COMPONENT_H__
