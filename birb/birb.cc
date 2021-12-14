
#include "a9e.h"
#include "components/playercontroller.h"
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

        SpawnPlayer(scene, {2, 2});

        scene.Run();

    } catch(const char* err){
        std::cout << std::string(err) << std::endl;
    }

    return 0;
}

