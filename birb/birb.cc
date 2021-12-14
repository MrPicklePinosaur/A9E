
#include "a9e.h"
#include "components/playercontroller.h"
#include "components/score.h"
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
        scene.RegisterSystem<ScoreSystem>();

        collider_system->SetCollidesWith(CollisionTag_Player, CollisionTag_Pipe);
        collider_system->SetCollidesWith(CollisionTag_Player, CollisionTag_PipeGap);

        SpawnPlayer(scene, {5, 2});
        pipemaker_system->StartSpawns(3s);

        scene.Run();

    } catch(const char* err){
        std::cout << std::string(err) << std::endl;
    }

    return 0;
}

