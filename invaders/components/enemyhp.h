#ifndef __ENEMYHEALTH_H__
#define __ENEMYHEALTH_H__

#include "a9e.h"

struct EnemyHp {
    int health;
};

class EnemyHpSystem final : public System
{
public:
    EnemyHpSystem(Scene& scene): System{scene} {}
    ~EnemyHpSystem() {}
    void BeforeUpdate() override {}
    void OnUpdate() override;
    void AfterUpdate() override {}
};

#endif // __ENEMYHEALTH_H__
