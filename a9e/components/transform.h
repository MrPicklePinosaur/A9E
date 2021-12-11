#ifndef __A9E_TRANSFORM_H__
#define __A9E_TRANSFORM_H__

#include "../math/vec.h"

struct Transform {
    vec2 pos;
    int layer = 0;
};

#endif // __A9E_TRANSFORM_H__
