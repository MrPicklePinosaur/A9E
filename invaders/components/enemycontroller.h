#ifndef __ENEMYCONTROLLER_H__
#define __ENEMYCONTROLLER_H__

#include "a9e.h"

struct EnemyController {
    float speed = 10.0f;
    int firerate = 100;
    void (*onShoot)(Scene&,Entity) = nullptr;
};

class EnemyControllerSystem : public System
{
public:
    EnemyControllerSystem(Scene& scene): System{scene} {}
    ~EnemyControllerSystem() {}
    void BeforeUpdate() override {}
    void OnUpdate() override;
    void AfterUpdate() override {}
};

#endif // __ENEMYCONTROLLER_H__
