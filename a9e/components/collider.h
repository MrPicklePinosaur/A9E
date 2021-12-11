#ifndef __A9E_COLLIDER_H__
#define __A9E_COLLIDER_H__

#include <memory>
#include <bitset>

#include "../ecs.h"
#include "../math/vec.h"
#include "transform.h"

const int MAX_COLLIDER_TAGS = 32;

class ColData;
class BoxColData;
class SphereColData;

struct Collider {
    // TODO try getting this to work with unique ptr
    std::shared_ptr<ColData> data;
    std::bitset<MAX_COLLIDER_TAGS> tags;
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
    inline CollisionData CheckCollide(const Transform& trans, const ColData* other_col, const Transform& other_trans) const override {
        return other_col->CheckCollide(other_trans, this, trans);
    }
    inline CollisionData CheckCollide(const Transform& trans, const BoxColData* other_col, const Transform& other_trans) const override {
        return ColTestBoxVBox(this, trans, other_col, other_trans);
    }
    inline CollisionData CheckCollide(const Transform& trans, const SphereColData* other_col, const Transform& other_trans) const override {
        return CollisionData{.isCollision = false};
    }
};

class SphereColData : public ColData
{
public:
    float radius;
    vec2 pos;
    SphereColData(float radius): ColData{}, radius{radius}, pos{vec2::zero()} {}
    SphereColData(float radius, const vec2& pos): ColData{}, radius{radius}, pos{pos} {}
    ~SphereColData() {};
    inline CollisionData CheckCollide(const Transform& trans, const ColData* other_col, const Transform& other_trans) const override {
        return other_col->CheckCollide(other_trans, this, trans);
    }
    inline CollisionData CheckCollide(const Transform& trans, const BoxColData* other_col, const Transform& other_trans) const override {
        return CollisionData{.isCollision = false};
    }
    inline CollisionData CheckCollide(const Transform& trans, const SphereColData* other_col, const Transform& other_trans) const override {
        return ColTestSphereVSphere(this, trans, other_col, other_trans);
    }
};

class ColliderSystem : public System
{
    void (*contactCallback)(Scene&, Entity, Entity) = nullptr;
public:
    ColliderSystem(Scene& scene);
    ~ColliderSystem();
    void SetContactCallback(void (*contactCallback)(Scene&, Entity, Entity));
    void BeforeUpdate() override;
    void OnUpdate() override;
    void AfterUpdate() override;
};

#endif // __A9E_COLLIDER_H__
