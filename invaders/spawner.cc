
#include <vector>
#include "spawner.h"
#include "common.h"
#include "components/enemycontroller.h"
#include "components/playercontroller.h"
#include "components/playerhp.h"
#include "components/score.h"

void SpawnPlayer(Scene& scene, const vec2& pos, const vec2& dir)
{
    float playerSpeed = 20.0f;
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {.render_type = RenderType_Char, .data = RenderChar{'A'}, .render_style = RenderStyle_Player});
    scene.AddComponent<PhysicsBody>(e, {
        .mass = 10.0f,
        .isSimulated = true,
        .useGravity = false,
        .cleanOffScreen = false,
        .velocity = dir*playerSpeed
    });
    scene.AddComponent<PlayerController>(e, {.speed = playerSpeed});
    scene.AddComponent<PlayerHp>(e, PlayerHp{});
    scene.AddComponent<PlayerScore>(e, PlayerScore{});
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

void SpawnPlayerBullet(Scene& scene, const vec2& pos, const vec2& dir)
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
        .onCollide = [](Scene& scene, Entity self, Entity other) {
            scene.DestroyEntity(self);
            // give player score
            ScoreReward& score_reward = scene.GetComponent<ScoreReward>(other);
            for (auto& e : scene.MakeEntityView<PlayerScore>()) {
                PlayerScore& player_score = scene.GetComponent<PlayerScore>(e);
                player_score.score += score_reward.reward;
            }

        }
    });
}

void SpawnBasicEnemy(Scene& scene, const vec2& pos, const vec2& dir)
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
    scene.AddComponent<ScoreReward>(e, {10});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Enemy,
        .onCollide = [](Scene& scene, Entity self, Entity other) {
            Collider& col_other = scene.GetComponent<Collider>(other);
            if (col_other.collider_id == CollisionTag_PlayerBullet) scene.DestroyEntity(self);
        }
    });
}

void SpawnBasicEnemyBullet(Scene& scene, const vec2& pos, const vec2& dir)
{
    float bulletSpeed = 10.0f;
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {.render_type = RenderType_Char, .data = RenderChar{'o'}, .render_style = RenderStyle_BasicEnemyBullet});
    scene.AddComponent<PhysicsBody>(e, {.velocity = dir*bulletSpeed});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_EnemyBullet,
        .isTrigger = true,
        .onCollide = [](Scene& scene, Entity self, Entity other) { scene.DestroyEntity(self); }
    });
}

void SpawnTwinGunnerEnemy(Scene& scene, const vec2& pos, const vec2& dir)
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
    scene.AddComponent<ScoreReward>(e, {20});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{-1.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Enemy,
        .onCollide = [](Scene& scene, Entity self, Entity other) {
            Collider& col_other = scene.GetComponent<Collider>(other);
            if (col_other.collider_id == CollisionTag_PlayerBullet) scene.DestroyEntity(self);
        }
    });
}

void SpawnMachineGunnerEnemy(Scene& scene, const vec2& pos, const vec2& dir)
{
    float enemySpeed = 12.0f;
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Bitmap, RenderBitmap{{{'|',-1,0},{'T',0,0},{'|',1,0}}}});
    scene.AddComponent<PhysicsBody>(e, {.velocity = dir*enemySpeed});
    scene.AddComponent<EnemyController>(e, {
        .firerate = 200, 
        .onShoot = [](Scene& scene, Entity self) {
            Transform& transform = scene.GetComponent<Transform>(self);
            SpawnBasicEnemyBullet(scene, transform.pos+vec2{0.0f, 1.0f});
        }
    });
    scene.AddComponent<ScoreReward>(e, {40});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{-1.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Enemy,
        .onCollide = [](Scene& scene, Entity self, Entity other) {
            Collider& col_other = scene.GetComponent<Collider>(other);
            if (col_other.collider_id == CollisionTag_PlayerBullet) scene.DestroyEntity(self);
        }
    });
}

void SpawnBomberEnemy(Scene& scene, const vec2& pos, const vec2& dir)
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
    scene.AddComponent<ScoreReward>(e, {30});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{-1.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Enemy,
        .onCollide = [](Scene& scene, Entity self, Entity other) {
            Collider& col_other = scene.GetComponent<Collider>(other);
            if (col_other.collider_id == CollisionTag_PlayerBullet) scene.DestroyEntity(self);
        }
    });
}

void SpawnBomberEnemyBullet(Scene& scene, const vec2& pos, const vec2& dir)
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

void SpawnChargerEnemy(Scene& scene, const vec2& pos, const vec2& dir)
{
    float enemySpeed = 15.0f;
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Bitmap, RenderBitmap{{{'\\',-1,-1},{'V',0,0},{'/',1,-1},{'|',0,-1}}}});
    scene.AddComponent<PhysicsBody>(e, {.velocity = dir*enemySpeed});
    scene.AddComponent<EnemyController>(e, {
        .firerate = 300,
        .onShoot = [](Scene& scene, Entity self) {
            Transform& transform = scene.GetComponent<Transform>(self);
            SpawnChargerEnemyBullet(scene, transform.pos+vec2{0.0f, 1.0f});
        }
    });
    scene.AddComponent<ScoreReward>(e, {30});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Enemy,
        .onCollide = [](Scene& scene, Entity self, Entity other) {
            Collider& col_other = scene.GetComponent<Collider>(other);
            if (col_other.collider_id == CollisionTag_PlayerBullet) scene.DestroyEntity(self);
        }
    });
}

void SpawnChargerEnemyBullet(Scene& scene, const vec2& pos, const vec2& dir)
{
    float bulletSpeed = 3.0f;

    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'V'}});
    scene.AddComponent<PhysicsBody>(e, {.velocity = dir*bulletSpeed});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{-1.0f, -1.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_EnemyBullet,
        .isTrigger = true,
        .onCollide = [](Scene& scene, Entity self, Entity other) { scene.DestroyEntity(self); }
    });
}

void SpawnStarfishEnemy(Scene& scene, const vec2& pos, const vec2& dir)
{
    float enemySpeed = 10.0f;

    RenderBitmap bm = RenderBitmap{{
        {'\\',-1,-1},{'^',0,-1},{'/', 1,-1},
        {'<' ,-1, 0},{'$',0, 0},{'>', 1, 0},
        {'/' ,-1, 1},{'v',0, 1},{'\\',1, 1}
    }};
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Bitmap, bm});
    scene.AddComponent<PhysicsBody>(e, {.velocity = dir*enemySpeed});
    scene.AddComponent<EnemyController>(e, {
        .firerate = 50,
        .onShoot = [](Scene& scene, Entity self) {
            Transform& transform = scene.GetComponent<Transform>(self);
            std::vector<vec2> dirs = {{0.0f,-1.0f},{1.0f,-1.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f},{-1.0f,1.0f},{-1.0f,0.0f},{-1.0f,-1.0f}};
            for (auto& d : dirs) SpawnStarfishEnemyBullet(scene, transform.pos+d, d);
        }
    });
    scene.AddComponent<ScoreReward>(e, {60});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Enemy,
        .onCollide = [](Scene& scene, Entity self, Entity other) {
            Collider& col_other = scene.GetComponent<Collider>(other);
            if (col_other.collider_id == CollisionTag_PlayerBullet) scene.DestroyEntity(self);
        }
    });
}

void SpawnStarfishEnemyBullet(Scene& scene, const vec2& pos, const vec2& dir)
{
    float bulletSpeed = 5.0f;

    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'*'}});
    scene.AddComponent<PhysicsBody>(e, {.velocity = dir*bulletSpeed});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{-1.0f, -1.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_EnemyBullet,
        .isTrigger = true,
        .onCollide = [](Scene& scene, Entity self, Entity other) { scene.DestroyEntity(self); }
    });
}
