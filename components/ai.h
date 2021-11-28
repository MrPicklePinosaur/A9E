#ifndef __AI_H__
#define __AI_H__

#include "../ecs.h"

class AISystem : public System
{
public:
    AISystem(Scene& scene);
    ~AISystem();
    void BeforeUpdate() override;
    void OnUpdate() override;
    void AfterUpdate() override;
};

AISystem::AISystem(Scene& scene): System{scene} {}
AISystem::~AISystem() {}

void AISystem::BeforeUpdate() {}

void
AISystem::OnUpdate()
{
    for (auto& e : scene.MakeEntityView<Transform>()) {
        Transform& transform = scene.GetComponent<Transform>(e);
        transform.pos.x += 1;
    }
}

void AISystem::AfterUpdate() {}

#endif // __AI_H__
