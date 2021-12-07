#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include <memory>
#include <variant>
#include "../ecs.h"
#include "../math/vec.h"
#include "physicsbody.h"

class ColData;
class BoxColData;
class SphereColData;

struct Collider {
    // TODO try getting this to work with unique ptr
    std::shared_ptr<ColData> data;
    bool isTrigger = false;
};

struct CollisionData {
    vec2 normal; // vector with tail at a and tip at b
    float depth = 0.0f;
    bool isCollision = false;
};

struct Collision {
    Entity a;
    Entity b;
    CollisionData collision_data;
};

CollisionData ColTestBoxVBox(const BoxColData* a_col, const Transform& a_trans, const BoxColData* b_col, const Transform& b_trans);
// either write a ColTestSphereVBox function or just use the negative of the normal
CollisionData ColTestBoxVSphere(const BoxColData* a_col, const Transform& a_trans, const SphereColData* b_col, const Transform& b_trans);
CollisionData ColTestSphereVSphere(const SphereColData* a_col, const Transform& a_trans, const SphereColData* b_col, const Transform& b_trans);

class ColData
{
public:
    virtual ~ColData() = default;
    virtual CollisionData CheckCollide(const Transform& trans, const ColData* other_col, const Transform& other_trans) const = 0;
    virtual CollisionData CheckCollide(const Transform& trans, const BoxColData* other_col, const Transform& other_trans) const = 0;
    virtual CollisionData CheckCollide(const Transform& trans, const SphereColData* other_col, const Transform& other_trans) const = 0;
};

class BoxColData : public ColData
{
public:
    vec2 tl; // top left (0,0)
    vec2 br; // bottom right (1,1)
    BoxColData(const vec2& tl, const vec2& br): ColData{}, tl{tl}, br{br} {}
    ~BoxColData() {};
    CollisionData CheckCollide(const Transform& trans, const ColData* other_col, const Transform& other_trans) const override {
        return other_col->CheckCollide(other_trans, this, trans);
    }
    CollisionData CheckCollide(const Transform& trans, const BoxColData* other_col, const Transform& other_trans) const override {
        return ColTestBoxVBox(this, trans, other_col, other_trans);
    }
    CollisionData CheckCollide(const Transform& trans, const SphereColData* other_col, const Transform& other_trans) const override {
    }
};

class SphereColData : public ColData
{
public:
    float radius;
    vec2 pos;
    SphereColData(float radius, const vec2& pos): ColData{}, radius{radius}, pos{pos} {}
    ~SphereColData() {};
    CollisionData CheckCollide(const Transform& trans, const ColData* other_col, const Transform& other_trans) const override {
        return other_col->CheckCollide(other_trans, this, trans);
    }
    CollisionData CheckCollide(const Transform& trans, const BoxColData* other_col, const Transform& other_trans) const override {
    }
    CollisionData CheckCollide(const Transform& trans, const SphereColData* other_col, const Transform& other_trans) const override {
        return ColTestSphereVSphere(this, trans, other_col, other_trans);
    }
};

class ColliderSystem : public System
{
    void (*contactCallback)(Entity, Entity) = nullptr;
public:
    ColliderSystem(Scene& scene);
    ~ColliderSystem();
    void SetContactCallback(void (*contactCallback)(Entity, Entity));
    void BeforeUpdate() override;
    void OnUpdate() override;
    void AfterUpdate() override;
};

ColliderSystem::ColliderSystem(Scene& scene): System{scene} {}
ColliderSystem::~ColliderSystem() {}

void
ColliderSystem::SetContactCallback(void (*contactCallback)(Entity, Entity))
{
    this->contactCallback = contactCallback;
}

void
ColliderSystem::BeforeUpdate()
{
}

void
ColliderSystem::OnUpdate()
{
    std::vector<Collision> collisions;

    // find all possible collisions
    for (auto& e1 : scene.MakeEntityView<Transform,PhysicsBody,Collider>()) {
        Collider& e1_col = scene.GetComponent<Collider>(e1);
        Transform& e1_trans = scene.GetComponent<Transform>(e1);

        for (auto& e2 : scene.MakeEntityView<Transform,PhysicsBody,Collider>()) {
            if (e1 == e2) break;

            Collider& e2_col = scene.GetComponent<Collider>(e2);
            Transform& e2_trans = scene.GetComponent<Transform>(e2);

            CollisionData collision_data = e1_col.data->CheckCollide(e1_trans, e2_col.data.get(), e2_trans);
            if (collision_data.isCollision)
                collisions.push_back(Collision{.a = e1, .b = e2, .collision_data = collision_data});
        }
    }

    // calculate new velocities
    for (auto& c : collisions) {
        PhysicsBody& pb_a = scene.GetComponent<PhysicsBody>(c.a);
        PhysicsBody& pb_b = scene.GetComponent<PhysicsBody>(c.b);
    }

    // call the callback for each collision
    if (contactCallback != nullptr) {
        for (auto& c : collisions) contactCallback(c.a, c.b);
    }

}

void ColliderSystem::AfterUpdate() {}

CollisionData
ColTestBoxVBox(const BoxColData* a_col, const Transform& a_trans, const BoxColData* b_col, const Transform& b_trans)
{
    /* return ( */
    /*     a.tl.x < b.br.x && */
    /*     a.br.x < b.tl.x && */
    /*     a.tl.y < b.br.y && */
    /*     a.br.y < b.tl.y */
    /* ); */
    return {.isCollision = false};
}

CollisionData
ColTestBoxVSphere(const BoxColData* a_col, const Transform& a_trans, const SphereColData* b_col, const Transform& b_trans)
{
    return {.isCollision = false};
}

CollisionData
ColTestSphereVSphere(const SphereColData* a_col, const Transform& a_trans, const SphereColData* b_col, const Transform& b_trans)
{
    vec2 diff = (b_col->pos+b_trans.pos)-(a_col->pos+a_trans.pos);
    float depth = (a_col->radius+b_col->radius)-diff.magnitude();
    if (depth > 0) {
        return {.normal = vec2::normalize(diff), .depth = depth, .isCollision = true};
    }
    return {.isCollision = false};
}

#endif // __COLLIDER_H__
