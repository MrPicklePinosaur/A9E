#ifndef __PHYSICSBODY_H__
#define __PHYSICSBODY_H__

#include "../ecs.h"
#include "../math/vec.h"
#include "../config.h"
#include "transform.h"

struct PhysicsBody {
    float mass = 1.0f;
    float gravityScale = 1.0f;
    float linearDampening = 0.0f;
    float restitution = 1.0f;
    bool isSimulated = true;
    bool useGravity = true;
    vec2 velocity;
    vec2 force;
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

        if (!physics_body.isSimulated) continue;

        if (physics_body.useGravity)
            physics_body.force.y += (GRAVITY*physics_body.gravityScale)*physics_body.mass;

        physics_body.velocity += (physics_body.force/physics_body.mass)*scene.delta;
        transform.pos += physics_body.velocity*scene.delta;

        physics_body.force = vec2::zero();
    }
}

void PhysicsSystem::AfterUpdate() {}

#endif // __PHYSICSBODY_H__
