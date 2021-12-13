
#include "a9e.h"
#include "components/playercontroller.h"
#include "components/lives.h"
#include "spawner.h"
#include "common.h"

int
main(int argc, char** argv)
{
    try {

        Scene scene;

        scene.RegisterSystem<RenderSystem>();
        scene.RegisterSystem<PhysicsSystem>();
        auto* collider_system = scene.RegisterSystem<ColliderSystem>();
        scene.RegisterSystem<PlayerControllerSystem>();
        scene.RegisterSystem<LivesSystem>();

        collider_system->SetCollidesWith(CollisionTag_Player, CollisionTag_Ball);
        collider_system->SetCollidesWith(CollisionTag_Ball, CollisionTag_Wall);

        {
            Entity e = scene.CreateEntity();
            scene.AddComponent<Transform>(e, {.pos = {2, 2}});
            scene.AddComponent<Render>(e, {RenderType_Char, RenderChar{'O'}});
            scene.AddComponent<PhysicsBody>(e, {
                .mass = 1.0f,
                .isSimulated = true,
                .useGravity = false,
                .cleanOffScreen = false,
                .velocity = {5.0f, 5.0f}
            });
            scene.AddComponent<Collider>(e, {
                .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
                .collider_id = CollisionTag_Ball
            });
            scene.AddComponent<Lives>(e, {});
        }

        SpawnWalls(scene);

        {
            int length = 10;

            Entity e = scene.CreateEntity();
            scene.AddComponent<Transform>(e, {.pos = {2, 20}});
            scene.AddComponent<Render>(e, {RenderType_Box, RenderBox{'X', length, 1}});
            scene.AddComponent<PhysicsBody>(e, {
                .mass = 10000.0f,
                .isSimulated = true,
                .useGravity = false,
                .cleanOffScreen = false
            });
            scene.AddComponent<PlayerController>(e, {.speed = 10.0f});
            scene.AddComponent<Collider>(e, {
                .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{length, 2}),
                .collider_id = CollisionTag_Player
            });
        }

        scene.Run();

    } catch(const char* err){
        std::cout << std::string(err) << std::endl;
    }

    return 0;
}

