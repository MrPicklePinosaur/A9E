
#include "../a9e/a9e.h"
#include "components/playercontroller.h"
#include "components/enemycontroller.h"
#include "components/global.h"

void
contactCallback(Scene& scene, Entity a, Entity b)
{
    /* std::cout << "COLLISION BETWEEN " << a << " AND " << b << std::endl; */
    /* scene.DestroyEntity(a); */
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
    scene.RegisterSystem<GlobalSystem>();

    scene.setGlobal(GlobalState{});

    {
        Entity e = scene.CreateEntity();
        scene.AddComponent<Transform>(e, {{10, 10}});
        scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'V'}});
        scene.AddComponent<PhysicsBody>(e, {.mass = 10.0f, .isSimulated = true, .useGravity = false, .velocity = {10.0f, 0.0f}});
        scene.AddComponent<EnemyController>(e, {});
        scene.AddComponent<Collider>(e, {.data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f})});
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

