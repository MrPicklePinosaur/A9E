
#include "spawner.h"
#include "common.h"
#include "components/enemycontroller.h"
#include "components/playercontroller.h"
#include "components/playerhp.h"

void
SpawnPlayer(Scene& scene, const vec2& pos, const vec2& dir)
{
    float playerSpeed = 20.0f;
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'A'}});
    scene.AddComponent<PhysicsBody>(e, {
        .mass = 10.0f,
        .isSimulated = true,
        .useGravity = false,
        .cleanOffScreen = false,
        .velocity = dir*playerSpeed
    });
    scene.AddComponent<PlayerController>(e, {.speed = playerSpeed});
    scene.AddComponent<PlayerHp>(e, PlayerHp{});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Player,
        .onCollide = [](Scene& scene, Entity self, Entity other) {
            Collider& col_other = scene.GetComponent<Collider>(other);
            if (col_other.collider_id == CollisionTag_EnemyBullet) {
                PlayerHp& player_hp = scene.GetComponent<PlayerHp>(self);
                --player_hp.hp;
            }
        }
    });
}

void
SpawnPlayerBullet(Scene& scene, const vec2& pos, const vec2& dir)
{
    float bulletSpeed = 10.0f;
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'|'}});
    scene.AddComponent<PhysicsBody>(e, {.mass = 1.0f, .isSimulated = true, .useGravity = false, .velocity = dir*bulletSpeed});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_PlayerBullet,
        .isTrigger = true,
        .onCollide = [](Scene& scene, Entity self, Entity other) { scene.DestroyEntity(self); }
    });
}

void
SpawnBasicEnemy(Scene& scene, const vec2& pos, const vec2& dir)
{
    float enemySpeed = 10.0f;
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'V'}});
    scene.AddComponent<PhysicsBody>(e, {.velocity = dir*enemySpeed});
    scene.AddComponent<EnemyController>(e, {
        .onShoot = [](Scene& scene, Entity self) {
            Transform& transform = scene.GetComponent<Transform>(self);
            SpawnBasicEnemyBullet(scene, transform.pos+vec2{0.0f, 1.0f});
        }
    });
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
SpawnBasicEnemyBullet(Scene& scene, const vec2& pos, const vec2& dir)
{
    float bulletSpeed = 10.0f;
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'o'}});
    scene.AddComponent<PhysicsBody>(e, {.velocity = dir*bulletSpeed});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_EnemyBullet,
        .isTrigger = true,
        .onCollide = [](Scene& scene, Entity self, Entity other) { scene.DestroyEntity(self); }
    });
}

void
SpawnTwinGunnerEnemy(Scene& scene, const vec2& pos, const vec2& dir)
{
    float enemySpeed = 10.0f;
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Bitmap, RenderBitmap{{{'T',-1,0},{'V',0,0},{'T',1,0}}}});
    scene.AddComponent<PhysicsBody>(e, {.velocity = dir*enemySpeed});
    scene.AddComponent<EnemyController>(e, {
        .onShoot = [](Scene& scene, Entity self) {
            Transform& transform = scene.GetComponent<Transform>(self);
            SpawnBasicEnemyBullet(scene, transform.pos+vec2{-1.0f, 1.0f});
            SpawnBasicEnemyBullet(scene, transform.pos+vec2{1.0f, 1.0f});
        }
    });
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{-1.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Enemy,
        .onCollide = [](Scene& scene, Entity self, Entity other) {
            Collider& col_other = scene.GetComponent<Collider>(other);
            if (col_other.collider_id == CollisionTag_PlayerBullet) scene.DestroyEntity(self);
        }
    });
}

void
SpawnBomberEnemy(Scene& scene, const vec2& pos, const vec2& dir)
{
    float enemySpeed = 5.0f;
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Bitmap, RenderBitmap{{{'|',-1,0},{'O',0,0},{'|',1,0}}}});
    scene.AddComponent<PhysicsBody>(e, {.velocity = dir*enemySpeed});
    scene.AddComponent<EnemyController>(e, {
        .onShoot = [](Scene& scene, Entity self) {
            Transform& transform = scene.GetComponent<Transform>(self);
            SpawnBomberEnemyBullet(scene, transform.pos+vec2{0.0f, 1.0f});
        }
    });
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{-1.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Enemy,
        .onCollide = [](Scene& scene, Entity self, Entity other) {
            Collider& col_other = scene.GetComponent<Collider>(other);
            if (col_other.collider_id == CollisionTag_PlayerBullet) scene.DestroyEntity(self);
        }
    });
}

void
SpawnBomberEnemyBullet(Scene& scene, const vec2& pos, const vec2& dir)
{
    float bulletSpeed = 20.0f;
    RenderBitmap bm = RenderBitmap{{
        {'\'',-1,-1},{'^',0,-1},{'\'',1,-1},
        {'<' ,-1, 0},{'O',0, 0},{'>' ,1, 0},
        {'\'',-1, 1},{'v',0, 1},{'\'',1, 1}
    }};

    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Bitmap, bm});
    scene.AddComponent<PhysicsBody>(e, {.velocity = dir*bulletSpeed});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{-1.0f, -1.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_EnemyBullet,
        .isTrigger = true,
        .onCollide = [](Scene& scene, Entity self, Entity other) { scene.DestroyEntity(self); }
    });
}
