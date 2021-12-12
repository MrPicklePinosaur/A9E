#ifndef __COMMON_H__
#define __COMMON_H__

#include "a9e.h"

enum CollisionTag {
    CollisionTag_Player = 0,
    CollisionTag_PlayerBullet,
    CollisionTag_Enemy,
    CollisionTag_EnemyBullet
};

#endif // __COMMON_H__
