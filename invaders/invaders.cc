
#include <vector>
#include <chrono>

#include "a9e.h"
#include "components/playercontroller.h"
#include "components/enemycontroller.h"
#include "components/global.h"
#include "components/wave.h"
#include "spawner.h"
#include "common.h"

using namespace std::chrono_literals;

const std::vector<Wave> waves = {
    Wave{
        .spawns = {SpawnBasicEnemy},
        .count = 2,
        .pad = 10s,
        .stagger = 1s,
        .spawn_point = {2, 2}
    },
    Wave{
        .spawns = {SpawnBasicEnemy},
        .count = 20,
        .pad = 5s,
        .stagger = 1s,
        .spawn_point = {2, 5}
    },
};

void
contactCallback(Scene& scene, Entity a, Entity b)
{
    std::cout << "COLLISION BETWEEN " << a << " AND " << b << std::endl;
    Collider& col_a = scene.GetComponent<Collider>(a);
    Collider& col_b = scene.GetComponent<Collider>(b);

    if (col_a.collider_id == CollisionTag_PlayerBullet && col_b.collider_id == CollisionTag_Enemy) { scene.DestroyEntity(a); scene.DestroyEntity(b); }
    if (col_b.collider_id == CollisionTag_PlayerBullet && col_a.collider_id == CollisionTag_Enemy) { scene.DestroyEntity(a); scene.DestroyEntity(b); }
}

int
main(int argc, char** argv)
{
    Scene scene;

    scene.RegisterSystem<RenderSystem>();
    scene.RegisterSystem<PhysicsSystem>();
    auto* collider_system = scene.RegisterSystem<ColliderSystem>();
    scene.RegisterSystem<PlayerControllerSystem>();
    scene.RegisterSystem<EnemyControllerSystem>();
    auto* wave_system = scene.RegisterSystem<WaveSystem>();
    scene.RegisterSystem<GlobalSystem>();

    collider_system->SetContactCallback(contactCallback);
    collider_system->SetCollidesWith(CollisionTag_PlayerBullet, CollisionTag_Enemy);
    collider_system->SetCollidesWith(CollisionTag_EnemyBullet, CollisionTag_Player);
    wave_system->AddWaves(waves);
    scene.setGlobal(GlobalState{});

    SpawnPlayer(scene, vec2{10, 20});

    scene.Run();

    return 0;
}

