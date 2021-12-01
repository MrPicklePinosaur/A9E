#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include <variant>
#include "../ecs.h"
#include "../math/vec.h"

enum ColliderType {
    ColliderType_None = 0,
    ColliderType_Box,
    ColliderType_Sphere
};
struct ColliderBox {
    vec2 min;
    vec2 max;
};
struct ColliderSphere {
    float radius;
};

struct Collider {
    ColliderType collider_type;
    std::variant<std::monostate, ColliderBox, ColliderSphere> data;
    bool isTrigger = false;
};

bool ColTestBoxVBox(const ColliderBox& a, const ColliderBox& b);
bool ColTestBoxVSphere(const ColliderBox& a, const ColliderSphere& b);
bool ColTestSphereVSphere(const ColliderSphere& a, const ColliderSphere& b);

class CollisionSystem : public System
{
public:
    CollisionSystem(Scene& scene);
    ~CollisionSystem();
    void BeforeUpdate() override;
    void OnUpdate() override;
    void AfterUpdate() override;
};

CollisionSystem::CollisionSystem(Scene& scene): System{scene} {}
CollisionSystem::~CollisionSystem() {}

void
CollisionSystem::BeforeUpdate()
{
}

void
CollisionSystem::OnUpdate()
{
    for (auto& e : scene.MakeEntityView<Transform,PhysicsBody.Collider>()) {
        Transform& transform = scene.GetComponent<Transform>(e);
        PhysicsBody& physics_body = scene.GetComponent<PhysicsBody>(e);
        Collider& collider = scene.GetComponent<Collider>(e);

    }
}

void CollisionSystem::AfterUpdate() {}

#endif // __COLLIDER_H__
