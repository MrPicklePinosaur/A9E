
#include "../spawner.h"
#include "playercontroller.h"

void
PlayerControllerSystem::OnUpdate()
{
    Inputer* inputer = scene.GetInputer();
    Renderer* renderer = scene.GetRenderer();

    for (auto& e : scene.MakeEntityView<Transform,PlayerController,PhysicsBody>()) {
        Transform& transform = scene.GetComponent<Transform>(e);
        PlayerController& player_controller = scene.GetComponent<PlayerController>(e);
        PhysicsBody& physics_body = scene.GetComponent<PhysicsBody>(e);

        if (inputer->GetKeyDown('a')) physics_body.velocity = vec2{-1*player_controller.speed, 0.0f};
        if (inputer->GetKeyDown('d')) physics_body.velocity = vec2{player_controller.speed, 0.0f};
        if (inputer->GetKeyDown('i')) player_controller.autofire = !player_controller.autofire;
        if (inputer->GetKeyDown(' ') || player_controller.autofire) {
            SpawnPlayerBullet(scene, transform.pos+vec2{0.0f, -1.0f});
        }

        // wrap player position 
        if (transform.pos.x < 0) transform.pos.x = renderer->GetScreenWidth();
        if (transform.pos.x > renderer->GetScreenWidth()) transform.pos.x = 0;
    }
}
