
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
    wave_system->AddWaves(waves);
    scene.setGlobal(GlobalState{});

    {
        Entity e = scene.CreateEntity();
        scene.AddComponent<Transform>(e, {{10, 20}});
        scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'A'}});
        scene.AddComponent<PhysicsBody>(e, {.mass = 10.0f, .isSimulated = true, .useGravity = false});
        scene.AddComponent<PlayerController>(e, {.speed = 20.0f});
        scene.AddComponent<Collider>(e, {.data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}), .collider_id = CollisionTag_Player});
    }

    scene.Run();

    return 0;
}

