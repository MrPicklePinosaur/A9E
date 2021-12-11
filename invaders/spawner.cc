
#include "spawner.h"
#include "components/enemycontroller.h"

void
SpawnBasicEnemy(Scene& scene, const vec2& pos)
{
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'V'}});
    scene.AddComponent<PhysicsBody>(e, {.mass = 10.0f, .isSimulated = true, .useGravity = false, .velocity = {10.0f, 0.0f}});
    scene.AddComponent<EnemyController>(e, {});
    scene.AddComponent<Collider>(e, {.data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f})});
}

void
SpawnPlayerBullet(Scene& scene, const vec2& pos)
{
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'|'}});
    scene.AddComponent<PhysicsBody>(e, {.mass = 1.0f, .isSimulated = true, .useGravity = false, .velocity = {0.0f, -10.0f}});
    /* scene.AddComponent<Collider>(e, {.data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f})}); */
}

void
SpawnEnemyBullet(Scene& scene, const vec2& pos)
{
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'o'}});
    scene.AddComponent<PhysicsBody>(e, {.mass = 1.0f, .isSimulated = true, .useGravity = false, .velocity = {0.0f, 10.0f}});
}
