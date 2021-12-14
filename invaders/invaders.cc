
#include <vector>
#include <string>
#include <chrono>

#include "a9e.h"
#include "components/ui/menucontroller.h"
#include "components/ui/exitcontroller.h"
#include "components/playercontroller.h"
#include "components/enemycontroller.h"
#include "components/playerhp.h"
#include "components/score.h"
#include "components/wave.h"
#include "spawner.h"
#include "common.h"

using namespace std::chrono_literals;

const std::vector<Wave> waves = {
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
        .stagger = 3s,
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
    Wave{
        .spawns = {SpawnChargerEnemy},
        .count = 1,
        .pad = 0s,
        .stagger = 1s,
        .spawn_point = {40, 0},
        .direction = {0.0f, 1.0f}
    },
    Wave{
        .spawns = {SpawnChargerEnemy},
        .count = 1,
        .pad = 0s,
        .stagger = 1s,
        .spawn_point = {20, 0},
        .direction = {0.0f, 1.0f}
    },
    Wave{
        .spawns = {SpawnChargerEnemy},
        .count = 1,
        .pad = 0s,
        .stagger = 1s,
        .spawn_point = {50, 0},
        .direction = {0.0f, 1.0f}
    },
    Wave{
        .spawns = {SpawnChargerEnemy},
        .count = 1,
        .pad = 0s,
        .stagger = 1s,
        .spawn_point = {35, 0},
        .direction = {0.0f, 1.0f}
    },
    Wave{
        .spawns = {SpawnStarfishEnemy},
        .count = 3,
        .pad = 1s,
        .stagger = 3s,
        .spawn_point = {0, 10},
        .direction = {1.0f, 0.0f}
    },
    Wave{
        .spawns = {SpawnBasicEnemy},
        .count = 2,
        .pad = 0s,
        .stagger = 500ms,
        .spawn_point = {0, 5},
        .direction = {1.0f, 0.0f}
    },
    Wave{
        .spawns = {SpawnBasicEnemy},
        .count = 2,
        .pad = 0s,
        .stagger = 500ms,
        .spawn_point = {80, 5},
        .direction = {-1.0f, 0.0f}
    },
    Wave{
        .spawns = {SpawnBasicEnemy},
        .count = 2,
        .pad = 0s,
        .stagger = 500ms,
        .spawn_point = {0, 5},
        .direction = {1.0f, 0.0f}
    },
    Wave{
        .spawns = {SpawnBasicEnemy},
        .count = 2,
        .pad = 0s,
        .stagger = 500ms,
        .spawn_point = {80, 5},
        .direction = {-1.0f, 0.0f}
    },
    Wave{
        .spawns = {SpawnBasicEnemy},
        .count = 2,
        .pad = 0s,
        .stagger = 500ms,
        .spawn_point = {0, 5},
        .direction = {1.0f, 0.0f}
    },
    Wave{
        .spawns = {SpawnBasicEnemy},
        .count = 2,
        .pad = 0s,
        .stagger = 500ms,
        .spawn_point = {80, 5},
        .direction = {-1.0f, 0.0f}
    }
};

int
main(int argc, char** argv)
{
    CursesRenderer renderer{false};
    CursesInputer inputer{};

    { // menu scene
        Scene scene{&renderer, &inputer};

        scene.RegisterSystem<RenderSystem>();
        scene.RegisterSystem<MenuControllerSystem>();

        {
            Entity e = scene.CreateEntity();
            scene.AddComponent<Transform>(e, {.pos = {10, 10}});
            scene.AddComponent<Render>(e, {RenderType_Text, RenderText{"[ Press Space to Begin ]"}});
        }

        scene.Run();
    }

    { // game scene
        Scene scene{&renderer, &inputer};

        scene.RegisterSystem<RenderSystem>();
        scene.RegisterSystem<ExitControllerSystem>();
        scene.RegisterSystem<PhysicsSystem>();
        auto* collider_system = scene.RegisterSystem<ColliderSystem>();
        scene.RegisterSystem<PlayerControllerSystem>();
        scene.RegisterSystem<PlayerHpSystem>();
        scene.RegisterSystem<PlayerScoreSystem>();
        scene.RegisterSystem<EnemyControllerSystem>();
        auto* wave_system = scene.RegisterSystem<WaveSystem>();

        collider_system->SetCollidesWith(CollisionTag_PlayerBullet, CollisionTag_Enemy);
        collider_system->SetCollidesWith(CollisionTag_EnemyBullet, CollisionTag_Player);
        wave_system->StartWaves(waves);
        SpawnPlayer(scene, vec2{10, 20}, {1.0f, 0.0f});

        scene.Run();
    }

    return 0;
}

