#ifndef __PLAYERCONTROLLER_H__
#define __PLAYERCONTROLLER_H__

#include "../../a9e/a9e.h"
#include "../spawner.h"

struct PlayerController {
    float speed;
    bool autofire = false;
};

class PlayerControllerSystem : public System
{
public:
    PlayerControllerSystem(Scene& scene): System{scene} {}
    ~PlayerControllerSystem() {}
    void BeforeUpdate() override {}
    void OnUpdate() override;
    void AfterUpdate() override {}
};

void
PlayerControllerSystem::OnUpdate()
{
    Inputer* inputer = scene.GetInputer();

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
    }
}

#endif // __PLAYERCONTROLLER_H__
