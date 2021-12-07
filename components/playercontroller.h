#ifndef __PLAYERCONTROLLER_H__
#define __PLAYERCONTROLLER_H__

#include "../ecs.h"
#include "../input.h"

struct PlayerController {
    float speed;
};

class PlayerControllerSystem : public System
{
    CursesInput& input_manager; // TODO this is temp, move this to scene later
public:
    PlayerControllerSystem(Scene& scene, CursesInput& input_manager);
    ~PlayerControllerSystem();
    void BeforeUpdate() override;
    void OnUpdate() override;
    void AfterUpdate() override;
};

PlayerControllerSystem::PlayerControllerSystem(Scene& scene, CursesInput& input_manager): System{scene}, input_manager{input_manager} {}
PlayerControllerSystem::~PlayerControllerSystem() {}

void PlayerControllerSystem::BeforeUpdate() {}

void
PlayerControllerSystem::OnUpdate()
{
    for (auto& e : scene.MakeEntityView<Transform,PlayerController>()) {
        Transform& transform = scene.GetComponent<Transform>(e);
        PlayerController& player_controller = scene.GetComponent<PlayerController>(e);

        if (input_manager.GetKeyDown('a')) transform.pos.x -= player_controller.speed;
        if (input_manager.GetKeyDown('d')) transform.pos.x += player_controller.speed;
        if (input_manager.GetKeyDown('w')) transform.pos.y -= player_controller.speed;
        if (input_manager.GetKeyDown('s')) transform.pos.y += player_controller.speed;
    }
}

void PlayerControllerSystem::AfterUpdate() {}

#endif // __PLAYERCONTROLLER_H__
