
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
        .count = 10,
        .pad = 5s,
        .stagger = 1s,
        .spawn_point = {2, 2}
    }
};

void
contactCallback(Scene& scene, Entity a, Entity b)
{
    /* std::cout << "COLLISION BETWEEN " << a << " AND " << b << std::endl; */
    Collider& col_a = scene.GetComponent<Collider>(a);
    Collider& col_b = scene.GetComponent<Collider>(b);
}

int
main(int argc, char** argv)
{
    Scene scene;

    scene.RegisterSystem<RenderSystem>();
    scene.RegisterSystem<PhysicsSystem>();
    scene.RegisterSystem<ColliderSystem>();
    scene.RegisterSystem<PlayerControllerSystem>();
    scene.RegisterSystem<EnemyControllerSystem>();
    auto* wave_system = scene.RegisterSystem<WaveSystem>();
    scene.RegisterSystem<GlobalSystem>();

    wave_system->AddWaves(waves);
    scene.setGlobal(GlobalState{});

    {
        SpawnBasicEnemy(scene, {10, 10});
    }

    {
        Entity e = scene.CreateEntity();
        scene.AddComponent<Transform>(e, {{10, 30}});
        scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'A'}});
        scene.AddComponent<PhysicsBody>(e, {.mass = 10.0f, .isSimulated = true, .useGravity = false});
        scene.AddComponent<PlayerController>(e, {.speed = 20.0f});
        scene.AddComponent<Collider>(e, {.data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f})});
    }

    scene.Run();

    return 0;
}

