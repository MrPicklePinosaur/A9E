#ifndef __PLAYERCONTROLLER_H__
#define __PLAYERCONTROLLER_H__

#include "../ecs.h"
#include "../inputer.h"

struct PlayerController {
    float speed;
};

class PlayerControllerSystem : public System
{
public:
    PlayerControllerSystem(Scene& scene);
    ~PlayerControllerSystem();
    void BeforeUpdate() override;
    void OnUpdate() override;
    void AfterUpdate() override;
};

PlayerControllerSystem::PlayerControllerSystem(Scene& scene): System{scene} {}
PlayerControllerSystem::~PlayerControllerSystem() {}

void PlayerControllerSystem::BeforeUpdate() {}

void
PlayerControllerSystem::OnUpdate()
{
    Inputer* inputer = scene.GetInputer();

    for (auto& e : scene.MakeEntityView<Transform,PlayerController>()) {
        Transform& transform = scene.GetComponent<Transform>(e);
        PlayerController& player_controller = scene.GetComponent<PlayerController>(e);

        if (inputer->GetKeyDown('a')) transform.pos.x -= player_controller.speed;
        if (inputer->GetKeyDown('d')) transform.pos.x += player_controller.speed;
    }
}

void PlayerControllerSystem::AfterUpdate() {}

#endif // __PLAYERCONTROLLER_H__
