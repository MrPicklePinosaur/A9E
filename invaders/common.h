#ifndef __COMMON_H__
#define __COMMON_H__

enum CollisionTag {
    CollisionTag_Player       = 0 << 0,
    CollisionTag_PlayerBullet = 0 << 1,
    CollisionTag_Enemy        = 0 << 2,
    CollisionTag_EnemyBullet  = 0 << 3
};

#endif // __COMMON_H__
