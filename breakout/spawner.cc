
#include "common.h"
#include "spawner.h"

void SpawnBall(Scene& scene, vec2 pos)
{
}

void SpawnPlayer(Scene& scene, vec2 pos)
{
}

void SpawnWalls(Scene& scene)
{
    { // left wall
        Entity e = scene.CreateEntity();
        scene.AddComponent<Transform>(e, {.pos = {-2.0f, -2.0f}});
        scene.AddComponent<PhysicsBody>(e, { .mass = 100000.0f, .isSimulated = false, .cleanOffScreen = false });
        scene.AddComponent<Collider>(e, {
            .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{2.0f, 30.0f}),
            .collider_id = CollisionTag_Wall
        });
    }
    { // right wall
        Entity e = scene.CreateEntity();
        scene.AddComponent<Transform>(e, {.pos = {80.0f, -2.0f}});
        scene.AddComponent<PhysicsBody>(e, { .mass = 100000.0f, .isSimulated = false, .cleanOffScreen = false });
        scene.AddComponent<Collider>(e, {
            .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{2.0f, 30.0f}),
            .collider_id = CollisionTag_Wall
        });
    }
    { // top wall
        Entity e = scene.CreateEntity();
        scene.AddComponent<Transform>(e, {.pos = {0, -1.0f}});
        scene.AddComponent<PhysicsBody>(e, { .mass = 100000.0f, .isSimulated = false, .cleanOffScreen = false });
        scene.AddComponent<Collider>(e, {
            .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{80.0f, 2.0f}),
            .collider_id = CollisionTag_Wall
        });
    }
}

void SpawnLoseTrigger(Scene& scene)
{
}

