
#include "../a9e/a9e.h"

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

    {

    }

    scene.Run();

    return 0;
}
