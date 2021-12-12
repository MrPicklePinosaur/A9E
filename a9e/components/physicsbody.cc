
#include "../config.h"
#include "transform.h"
#include "physicsbody.h"

PhysicsSystem::PhysicsSystem(Scene& scene): System{scene} {}
PhysicsSystem::~PhysicsSystem() {}

void PhysicsSystem::BeforeUpdate() {}

void
PhysicsSystem::OnUpdate()
{
    Renderer* r = scene.GetRenderer();

    for (auto& e : scene.MakeEntityView<Transform,PhysicsBody>()) {
        Transform& transform = scene.GetComponent<Transform>(e);
        PhysicsBody& physics_body = scene.GetComponent<PhysicsBody>(e);

        // delete the entity of it's offscreen
        if (physics_body.cleanOffScreen && (transform.pos.x < 0 || transform.pos.x > r->GetScreenWidth() || transform.pos.y < 0 || transform.pos.y > r->GetScreenHeight())) {
            scene.DestroyEntity(e);
            continue;
        }

        if (!physics_body.isSimulated) continue;

        if (physics_body.useGravity)
            physics_body.force.y += (GRAVITY*physics_body.gravityScale)*physics_body.mass;

        physics_body.velocity += (physics_body.force/physics_body.mass)*scene.getDelta();

        // clamp max velocity
        if (physics_body.velocity.magnitude() > physics_body.maxSpeed && physics_body.maxSpeed > 0.0f)
            physics_body.velocity *= physics_body.maxSpeed/physics_body.velocity.magnitude();

        transform.pos += physics_body.velocity*scene.getDelta();

        physics_body.force = vec2::zero();
    }
}

void PhysicsSystem::AfterUpdate() {}
