#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include <memory>
#include <variant>
#include "../ecs.h"
#include "../math/vec.h"
#include "physicsbody.h"

struct Collider {
    std::unique_ptr<ColData> data;
    bool isTrigger = false;
};

struct CollisionPoints {
    vec2 point_a;
    vec2 point_b;
    bool isCollision = false;
};

struct Collision {
    Entity a;
    Entity b;
    CollisionPoints points;
};

class ColData
{
public:
    ColData();
    virtual ~ColData() = default;
    virtual CollisionPoints CheckCollide(const Transform& trans, const ColData* other_col, const Transform& other_trans) = 0;
    virtual CollisionPoints CheckCollide(const Transform& trans, const BoxColData* other_col, const Transform& other_trans) = 0;
    virtual CollisionPoints CheckCollide(const Transform& trans, const SphereColData* other_col, const Transform& other_trans) = 0;
};

class BoxColData : public ColData
{
    vec2 tl; // top left (0,0)
    vec2 br; // bottom right (1,1)
public:
    BoxColData(const vec2& tl, const vec2& br): ColData{}, tl{tl}, br{br} {}
    ~BoxColData() {};
    CollisionPoints CheckCollide(const Transform& trans, const ColData* other_col, const Transform& other_trans) override {
        return other_col->TestCollider(other_trans, this, trans);
    }
    CollisionPoints CheckCollide(const Transform& trans, const BoxColData* other_col, const Transform& other_trans) override {
    }
    CollisionPoints CheckCollide(const Transform& trans, const SphereColData* other_col, const Transform& other_trans) override {
    }
};

class SphereColData : public ColData
{
    float radius;
    vec2 pos;
public:
    SphereColData(float radius, const vec2& pos): ColData{}, radius{radius}, pos{pos} {}
    ~SphereColData() {};
    CollisionPoints CheckCollide(const Transform& trans, const ColData* other_col, const Transform& other_trans) override {
        return other_col->TestCollider(other_trans, this, trans);
    }
    CollisionPoints CheckCollide(const Transform& trans, const BoxColData* other_col, const Transform& other_trans) override {
    }
    CollisionPoints CheckCollide(const Transform& trans, const SphereColData* other_col, const Transform& other_trans) override {
    }
};

CollisionPoints ColTestBoxVBox(const ColliderBox& a, const ColliderBox& b);
CollisionPoints ColTestBoxVSphere(const ColliderBox& a, const ColliderSphere& b);
CollisionPoints ColTestSphereVSphere(const ColliderSphere& a, const ColliderSphere& b);

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
    // find all possible collisions
    for (auto& e1 : scene.MakeEntityView<Transform,PhysicsBody,Collider>()) {
        Collider& e1_col = scene.GetComponent<Collider>(e1);

        for (auto& e2 : scene.MakeEntityView<Transform,PhysicsBody,Collider>()) {
            if (e1 == e2) break;

            Collider& e2_col = scene.GetComponent<Collider>(e2);
        }
    }
}

void CollisionSystem::AfterUpdate() {}


CollisionPoints
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

/* CollisionPoints */
/* ColTestBoxVSphere(const Transform& a_trans, const Collider& a_col, const Transform& b_trans, const Collider& b_col) */
/* { */
/*     return false; */
/* } */

CollisionPoints
ColTestSphereVSphere(const BoxColData* a_col, const Transform& a_trans, const BoxColData* b_col, const Transform& b_trans)
{
    if (((a.pos+a_trans.pos)-(b.pos+b_trans.pos)).magnitude() <= a.radius+b.radius) {
        return {.isCollision = true};
    }
    return {.isCollision = false};
}

#endif // __COLLIDER_H__
