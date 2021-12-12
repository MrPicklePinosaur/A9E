
#include "cstdlib"
#include "../spawner.h"
#include "enemycontroller.h"

void
EnemyControllerSystem::OnUpdate()
{
    for (auto& e : scene.MakeEntityView<Transform,EnemyController,PhysicsBody>()) {
        Transform& transform = scene.GetComponent<Transform>(e);
        EnemyController& enemy_controller = scene.GetComponent<EnemyController>(e);
        PhysicsBody& physics_body = scene.GetComponent<PhysicsBody>(e);

        if (enemy_controller.onShoot != nullptr && rand() % 100 / scene.getDelta() < enemy_controller.firerate)
            enemy_controller.onShoot(scene, e);
        
    }
}

