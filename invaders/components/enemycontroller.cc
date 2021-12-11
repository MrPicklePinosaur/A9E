
#include "cstdlib"
/* #include "../spawner.h" */
#include "enemycontroller.h"

void
EnemyControllerSystem::OnUpdate()
{
    for (auto& e : scene.MakeEntityView<Transform,EnemyController,PhysicsBody>()) {
        Transform& transform = scene.GetComponent<Transform>(e);
        EnemyController& enemy_controller = scene.GetComponent<EnemyController>(e);
        PhysicsBody& physics_body = scene.GetComponent<PhysicsBody>(e);

        /* if (rand() % 30 == 1) SpawnEnemyBullet(scene, transform.pos); */
        
    }
}

