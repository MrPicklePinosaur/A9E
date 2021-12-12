#ifndef __A9E_PHYSICSBODY_H__
#define __A9E_PHYSICSBODY_H__

#include "../ecs.h"
#include "../math/vec.h"

struct PhysicsBody {
    float mass = 1.0f;
    float gravityScale = 1.0f;
    float linearDampening = 0.0f;
    float restitution = 1.0f;
    bool isSimulated = true;
    bool useGravity = false;
    bool cleanOffScreen = true;
    vec2 velocity;
    float maxSpeed = 0.0f; // maxSpeed <= 0 means no max speed
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

#endif // __A9E_PHYSICSBODY_H__
