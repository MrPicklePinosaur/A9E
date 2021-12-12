
#include <vector>
#include <string>
#include <chrono>

#include "a9e.h"
#include "components/playercontroller.h"
#include "components/enemycontroller.h"
#include "components/playerhp.h"
#include "components/score.h"
#include "components/wave.h"
#include "spawner.h"
#include "common.h"

using namespace std::chrono_literals;

const std::vector<Wave> waves = {
    Wave{},
    Wave{
        .spawns = {SpawnBomberEnemy},
        .count = 3,
        .pad = 5s,
        .stagger = 3s,
        .spawn_point = {0, 2},
        .direction = {1.0f, 0.0f}
    },
    Wave{
        .spawns = {SpawnBasicEnemy},
        .count = 5,
        .pad = 12s,
        .stagger = 1s,
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
};

int
main(int argc, char** argv)
{
    try {
        Scene scene;

        scene.RegisterSystem<RenderSystem>();
        scene.RegisterSystem<PhysicsSystem>();
        auto* collider_system = scene.RegisterSystem<ColliderSystem>();
        scene.RegisterSystem<PlayerControllerSystem>();
        scene.RegisterSystem<PlayerHpSystem>();
        scene.RegisterSystem<PlayerScoreSystem>();
        scene.RegisterSystem<EnemyControllerSystem>();
        auto* wave_system = scene.RegisterSystem<WaveSystem>();

        collider_system->SetCollidesWith(CollisionTag_PlayerBullet, CollisionTag_Enemy);
        collider_system->SetCollidesWith(CollisionTag_EnemyBullet, CollisionTag_Player);
        wave_system->AddWaves(waves);

        SpawnPlayer(scene, vec2{10, 20}, {1.0f, 0.0f});

        scene.Run();
    } catch(const char* err){
        std::cout << std::string(err) << std::endl;
    }

    return 0;
}

