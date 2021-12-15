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
    EnemyType_Charger,
    EnemyType_Starfish,
};

enum EnemyBulletType {
    EnemyBulletType_Basic = 0,
    EnemyTypeBullet_Bomb,
    EnemyTypeBullet_MachineGun
};

enum RenderStyle {
    RenderStyle_Player = 1,
    RenderStyle_BasicEnemyBullet,
    RenderStyle_StartText,
    RenderStyle_LogoText,
};

#endif // __COMMON_H__
