#ifndef __ENEMYCONTROLLER_H__
#define __ENEMYCONTROLLER_H__

#include "cstdlib"
#include "../spawner.h"

struct EnemyController {

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

void
EnemyControllerSystem::OnUpdate()
{
    for (auto& e : scene.MakeEntityView<Transform,EnemyController,PhysicsBody>()) {
        Transform& transform = scene.GetComponent<Transform>(e);
        EnemyController& enemy_controller = scene.GetComponent<EnemyController>(e);
        PhysicsBody& physics_body = scene.GetComponent<PhysicsBody>(e);

        if (rand() % 30 == 1) SpawnEnemyBullet(scene, transform.pos);
        
    }
}

#endif // __ENEMYCONTROLLER_H__
