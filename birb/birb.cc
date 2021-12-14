
#include "a9e.h"
#include "components/playercontroller.h"
#include "components/pipemaker.h"
#include "spawner.h"
#include "common.h"

using namespace std::chrono_literals;

int
main(int argc, char** argv)
{
    try {

        Scene scene;

        scene.RegisterSystem<RenderSystem>();
        scene.RegisterSystem<PhysicsSystem>();
        auto* collider_system = scene.RegisterSystem<ColliderSystem>();
        scene.RegisterSystem<PlayerControllerSystem>();
        auto* pipemaker_system = scene.RegisterSystem<PipeMakerSystem>();

        SpawnPlayer(scene, {5, 2});
        pipemaker_system->StartSpawns(3s);

        scene.Run();

    } catch(const char* err){
        std::cout << std::string(err) << std::endl;
    }

    return 0;
}

