
#include "playercontroller.h"

void
PlayerControllerSystem::OnUpdate()
{
    Inputer* inputer = scene.GetInputer();

    for (auto& e : scene.MakeEntityView<Transform,PlayerController,PhysicsBody>()) {
        Transform& transform = scene.GetComponent<Transform>(e);
        PlayerController& player_controller = scene.GetComponent<PlayerController>(e);
        PhysicsBody& physics_body = scene.GetComponent<PhysicsBody>(e);

        if (inputer->GetKeyDown(' ')) physics_body.velocity = vec2{0.0f, -1.0f}*player_controller.jump_impulse/physics_body.mass;

    }
}

