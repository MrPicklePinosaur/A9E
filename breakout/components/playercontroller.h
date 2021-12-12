#ifndef __PLAYERCONTROLLER_H__
#define __PLAYERCONTROLLER_H__

#include "a9e.h"

struct PlayerController {
    float speed;
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

#endif // __PLAYERCONTROLLER_H__