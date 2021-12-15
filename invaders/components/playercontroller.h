#ifndef __PLAYERCONTROLLER_H__
#define __PLAYERCONTROLLER_H__

#include <chrono>
#include "a9e.h"

struct PlayerController {
    float speed;
    std::chrono::milliseconds fire_cooldown;
};

class PlayerControllerSystem : public System
{
    std::chrono::steady_clock::time_point last_shoot;
public:
    PlayerControllerSystem(Scene& scene): System{scene} {}
    ~PlayerControllerSystem() {}
    void BeforeUpdate() override {}
    void OnUpdate() override;
    void AfterUpdate() override {}
};

#endif // __PLAYERCONTROLLER_H__
