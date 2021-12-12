
#include "spawner.h"
#include "common.h"
#include "components/enemycontroller.h"
#include "components/playercontroller.h"

void
SpawnPlayer(Scene& scene, const vec2& pos)
{
    float playerSpeed = 20.0f;
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'A'}});
    scene.AddComponent<PhysicsBody>(e, {.mass = 10.0f, .isSimulated = true, .useGravity = false, .cleanOffScreen = false, .velocity = vec2{playerSpeed, 0.0f}});
    scene.AddComponent<PlayerController>(e, {.speed = playerSpeed});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Player,
        .onCollide = [](Scene& scene, Entity self, Entity other) {
            Collider& col_other = scene.GetComponent<Collider>(other);
            if (col_other.collider_id == CollisionTag_EnemyBullet) {
                // take damage
            }
        }
    });
}

void
SpawnBasicEnemy(Scene& scene, const vec2& pos)
{
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'V'}});
    scene.AddComponent<PhysicsBody>(e, {.mass = 10.0f, .isSimulated = true, .useGravity = false, .velocity = {10.0f, 0.0f}});
    scene.AddComponent<EnemyController>(e, {});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Enemy,
        .onCollide = [](Scene& scene, Entity self, Entity other) {
            Collider& col_other = scene.GetComponent<Collider>(other);
            if (col_other.collider_id == CollisionTag_PlayerBullet) scene.DestroyEntity(self);
        }
    });
}

void
SpawnPlayerBullet(Scene& scene, const vec2& pos)
{
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'|'}});
    scene.AddComponent<PhysicsBody>(e, {.mass = 1.0f, .isSimulated = true, .useGravity = false, .velocity = {0.0f, -10.0f}});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_PlayerBullet,
        .isTrigger = true,
        .onCollide = [](Scene& scene, Entity self, Entity other) { scene.DestroyEntity(self); }
    });
}

void
SpawnEnemyBullet(Scene& scene, const vec2& pos)
{
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'o'}});
    scene.AddComponent<PhysicsBody>(e, {.mass = 1.0f, .isSimulated = true, .useGravity = false, .velocity = {0.0f, 10.0f}});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_EnemyBullet,
        .isTrigger = true,
        .onCollide = [](Scene& scene, Entity self, Entity other) { scene.DestroyEntity(self); }
    });

}

