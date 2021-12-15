
#include <vector>
#include <cstdlib>
#include "spawner.h"
#include "common.h"
#include "components/enemycontroller.h"
#include "components/playercontroller.h"
#include "components/enemyhp.h"
#include "components/playerhp.h"
#include "components/score.h"

auto enemyOnCollide = [](Scene& scene, Entity self, Entity other) {
    Collider& col_other = scene.GetComponent<Collider>(other);
    if (col_other.collider_id == CollisionTag_PlayerBullet) {
        EnemyHp& enemy_hp = scene.GetComponent<EnemyHp>(self);
        --enemy_hp.health;
    }
};

auto enemyBulletOnCollide = [](Scene& scene, Entity self, Entity other) {
    scene.DestroyEntity(self);
};

void SpawnPlayer(Scene& scene, const vec2& pos, const vec2& dir)
{
    using namespace std::chrono_literals;

    float playerSpeed = 20.0f;
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'A', RenderStyle_Player}});
    scene.AddComponent<PhysicsBody>(e, {
        .mass = 10.0f,
        .isSimulated = true,
        .useGravity = false,
        .cleanOffScreen = false,
        .velocity = dir*playerSpeed
    });
    scene.AddComponent<PlayerController>(e, {.speed = playerSpeed, .fire_cooldown = 300ms});
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
    float bulletSpeed = 20.0f;
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'|', RenderStyle_Player}});
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
    scene.AddComponent<EnemyHp>(e, {1});
    scene.AddComponent<ScoreReward>(e, {10});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Enemy,
        .onCollide = enemyOnCollide
    });
}

void SpawnBasicEnemyBullet(Scene& scene, const vec2& pos, const vec2& dir)
{
    float bulletSpeed = 10.0f;
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'o', RenderStyle_BasicEnemyBullet}});
    scene.AddComponent<PhysicsBody>(e, {.velocity = dir*bulletSpeed});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_EnemyBullet,
        .isTrigger = true,
        .onCollide = enemyBulletOnCollide
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
    scene.AddComponent<EnemyHp>(e, {1});
    scene.AddComponent<ScoreReward>(e, {20});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{-1.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Enemy,
        .onCollide = enemyOnCollide
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
    scene.AddComponent<EnemyHp>(e, {2});
    scene.AddComponent<ScoreReward>(e, {30});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{-1.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Enemy,
        .onCollide = enemyOnCollide
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
    scene.AddComponent<EnemyHp>(e, {3});
    scene.AddComponent<ScoreReward>(e, {50});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{-1.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Enemy,
        .onCollide = enemyOnCollide
    });
}

void SpawnBomberEnemyBullet(Scene& scene, const vec2& pos, const vec2& dir)
{
    float bulletSpeed = 20.0f;
    RenderBitmap bm = RenderBitmap{{
        {'\'',-1,-1,RenderStyle_BasicEnemyBullet},{'^',0,-1,RenderStyle_BasicEnemyBullet},{'\'',1,-1,RenderStyle_BasicEnemyBullet},
        {'<' ,-1, 0,RenderStyle_BasicEnemyBullet},{'O',0, 0,RenderStyle_BasicEnemyBullet},{'>' ,1, 0,RenderStyle_BasicEnemyBullet},
        {'\'',-1, 1,RenderStyle_BasicEnemyBullet},{'v',0, 1,RenderStyle_BasicEnemyBullet},{'\'',1, 1,RenderStyle_BasicEnemyBullet}
    }};

    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Bitmap, bm});
    scene.AddComponent<PhysicsBody>(e, {.velocity = dir*bulletSpeed});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{-1.0f, -1.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_EnemyBullet,
        .isTrigger = true,
        .onCollide = enemyBulletOnCollide
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
    scene.AddComponent<EnemyHp>(e, {1});
    scene.AddComponent<ScoreReward>(e, {20});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Enemy,
        .onCollide = enemyOnCollide
    });
}

void SpawnChargerEnemyBullet(Scene& scene, const vec2& pos, const vec2& dir)
{
    float bulletSpeed = 3.0f;

    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'V', RenderStyle_BasicEnemyBullet}});
    scene.AddComponent<PhysicsBody>(e, {.velocity = dir*bulletSpeed});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{-1.0f, -1.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_EnemyBullet,
        .isTrigger = true,
        .onCollide = enemyBulletOnCollide
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
    scene.AddComponent<EnemyHp>(e, {3});
    scene.AddComponent<ScoreReward>(e, {60});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Enemy,
        .onCollide = enemyOnCollide
    });
}

void SpawnStarfishEnemyBullet(Scene& scene, const vec2& pos, const vec2& dir)
{
    float bulletSpeed = 5.0f;

    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'*', RenderStyle_BasicEnemyBullet}});
    scene.AddComponent<PhysicsBody>(e, {.velocity = dir*bulletSpeed});
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{-1.0f, -1.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_EnemyBullet,
        .isTrigger = true,
        .onCollide = enemyBulletOnCollide
    });
}

std::vector<Wave>
MakeWaves()
{
    using namespace std::chrono_literals;

    std::vector<Wave> waves = {
        Wave{
            .spawns = {SpawnBasicEnemy},
            .count = 5,
            .pad = 12s,
            .stagger = 1500ms,
            .spawn_point = {0, 2},
            .direction = {1.0f, 0.0f}
        },
        Wave{
            .spawns = {SpawnTwinGunnerEnemy},
            .count = 10,
            .pad = 5s,
            .stagger = 1s,
            .spawn_point = {80, 5},
            .direction = {-1.0f, 0.0f}
        },
        Wave{
            .spawns = {SpawnBomberEnemy},
            .count = 3,
            .pad = 15s,
            .stagger = 5s,
            .spawn_point = {0, 2},
            .direction = {1.0f, 0.0f}
        },
        Wave{
            .spawns = {SpawnMachineGunnerEnemy},
            .count = 5,
            .pad = 10s,
            .stagger = 1s,
            .spawn_point = {80, 5},
            .direction = {-1.0f, 0.0f}
        },
    };

    // charger storm
    std::vector<int> spawn_pos = {40, 20, 50, 35, 10, 30};
    for (auto& i : spawn_pos)
        waves.push_back(
            Wave{
                .spawns = {SpawnChargerEnemy},
                .count = 1,
                .pad = 0s,
                .stagger = 1s,
                .spawn_point = {i, 0},
                .direction = {0.0f, 1.0f}
            }
        );

    // starfish
    waves.push_back(
        Wave{
            .spawns = {SpawnStarfishEnemy},
            .count = 3,
            .pad = 15s,
            .stagger = 3s,
            .spawn_point = {0, 10},
            .direction = {1.0f, 0.0f}
        }
    );

    // bunch of alternating normal enemies
    for (int i = 0; i < 4; ++i) {
        waves.push_back(
            Wave{
                .spawns = {SpawnBasicEnemy},
                .count = 2,
                .pad = 0s,
                .stagger = 500ms,
                .spawn_point = {0, 5},
                .direction = {1.0f, 0.0f}
            }
        );
        waves.push_back(
            Wave{
                .spawns = {SpawnBasicEnemy},
                .count = 2,
                .pad = 0s,
                .stagger = 500ms,
                .spawn_point = {80, 5},
                .direction = {-1.0f, 0.0f}
            }
        );

    }

    // alternate twin gunner + machine
    for (int i = 0; i < 6; ++i) {
        waves.push_back(
            Wave{
                .spawns = {SpawnTwinGunnerEnemy},
                .count = 1,
                .pad = 0s,
                .stagger = 1s,
                .spawn_point = {80, 9},
                .direction = {-1.0f, 0.0f}
            }
        );
        waves.push_back(
            Wave{
                .spawns = {SpawnMachineGunnerEnemy},
                .count = 1,
                .pad = 0s,
                .stagger = 1s,
                .spawn_point = {0, 7},
                .direction = {1.0f, 0.0f}
            }
        );
    }

    // charger storm again
    for (int i = 0; i < 10; ++i)
        waves.push_back(
            Wave{
                .spawns = {SpawnChargerEnemy},
                .count = 1,
                .pad = 0s,
                .stagger = 1s,
                .spawn_point = {rand() % 80, 0},
                .direction = {0.0f, 1.0f}
            }
        );

    // bunch of bombers
    for (int i = 0; i < 5; ++i) {
        waves.push_back(
            Wave{
                .spawns = {SpawnBomberEnemy},
                .count = 1,
                .pad = 3s,
                .stagger = 0s,
                .spawn_point = {0, rand() % 10 + 2},
                .direction = {1.0f, 0.0f}
            }
        );
        waves.push_back(
            Wave{
                .spawns = {SpawnBomberEnemy},
                .count = 1,
                .pad = 3s,
                .stagger = 0s,
                .spawn_point = {80, rand() % 10 + 2},
                .direction = {-1.0f, 0.0f}
            }
        );
    }

    // end
    waves.push_back(
        Wave{
            .spawns = {SpawnStarfishEnemy},
            .count = 1,
            .pad = 1s,
            .stagger = 1s,
            .spawn_point = {0, 2},
            .direction = {1.0f, 0.0f}
        }
    );

    return waves;

}
