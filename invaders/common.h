#ifndef __COMMON_H__
#define __COMMON_H__

#include "a9e.h"

enum CollisionTag {
    CollisionTag_Player = 0,
    CollisionTag_PlayerBullet,
    CollisionTag_Enemy,
    CollisionTag_EnemyBullet
};

enum EnemyType {
    EnemyType_Basic = 0,
    EnemyType_TwinGunner,
    EnemyType_Bomber,
    EnemyType_MachineGunner,
    EnemyType_Scout,
    EnemyType_Exploder
};

enum EnemyBulletType {
    EnemyBulletType_Basic = 0,
    EnemyTypeBullet_Bomb,
    EnemyTypeBullet_MachineGun
};

#endif // __COMMON_H__