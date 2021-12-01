#ifndef __PHYSICSBODY_H__
#define __PHYSICSBODY_H__

#include "../math/vec.h"
#include "transform.h"

struct PhysicsBody {
    float mass = 1.0f;
    float gravityScale = 1.0f;
    float linearDampening = 0.0f;
    bool isSimulated = true;
    bool useGravity = true;
    vec2 velocity;
    vec2 acceleration;
};

class PhysicsSystem : public System
{
public:
    PhysicsSystem(Scene& scene);
    ~PhysicsSystem();
    void BeforeUpdate() override;
    void OnUpdate() override;
    void AfterUpdate() override;
};

PhysicsSystem::PhysicsSystem(Scene& scene): System{scene} {}
PhysicsSystem::~PhysicsSystem() {}

void PhysicsSystem::BeforeUpdate() {}

void
PhysicsSystem::OnUpdate()
{
    for (auto& e : scene.MakeEntityView<Transform,PhysicsBody>()) {
        Transform& transform = scene.GetComponent<Transform>(e);
        PhysicsBody& physics_body = scene.GetComponent<PhysicsBody>(e);

        physics_body.velocity += physics_body.acceleration*TIME_STEP;
        transform.pos += physics_body.velocity*TIME_STEP;
    }
}

void PhysicsSystem::AfterUpdate() {}

#endif // __PHYSICSBODY_H__
