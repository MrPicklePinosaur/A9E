
#include <cstdlib>
#include <iostream>
#include <string>
#include "a9e.h"

enum CollisionTag {
    CollisionTag_Floor = 1,
    CollisionTag_Ball
};

void
HelpMessage()
{
    std::cout << "USAGE:" << std::endl;
    std::cout << "-m [float]   ball mass (default value: 1.0)" << std::endl;
    std::cout << "-b [int]     ball count (default value: 40)" << std::endl;
    std::cout << "-g [float]   gravity (default value: 9.0)" << std::endl;
    std::cout << "-r [float]   restitution (default value: 0.9)" << std::endl;
}

int
main(int argc, char** argv)
{
    // read flags for physics
    float BALL_MASS = 1.0f;
    int BALL_COUNT = 40;
    float GRAVITY = 9.0f;
    float RESTITUTION = 0.9f;

    try {
        for (int i = 1; i < argc; ++i) {
            if (i+1 >= argc) throw "invalid";

            if (std::string("-m").compare(argv[i]) == 0) BALL_MASS = std::stof(argv[++i]);
            else if (std::string("-b").compare(argv[i]) == 0) BALL_COUNT = std::stoi(argv[++i]);
            else if (std::string("-g").compare(argv[i]) == 0) GRAVITY = std::stof(argv[++i]);
            else if (std::string("-r").compare(argv[i]) == 0) RESTITUTION = std::stof(argv[++i]);
            else throw "invalid";
        }
    } catch(...) {
        HelpMessage();
        return 1;
    }

    CursesRenderer renderer{false};
    CursesInputer inputer{};

    srand(time(NULL));

    {
        Scene scene{&renderer, &inputer};
        scene.RegisterSystem<RenderSystem>();
        auto* physics_system = scene.RegisterSystem<PhysicsSystem>();
        auto* collider_system = scene.RegisterSystem<ColliderSystem>();
        
        physics_system->SetGravity(GRAVITY);
        collider_system->SetCollidesWith(CollisionTag_Ball, CollisionTag_Floor);

        {
            Entity e = scene.CreateEntity();
            Transform t{{2, 20}};
            Render r{RenderType_Box, RenderBox{'/', 80, 5}};
            PhysicsBody pb{.mass = 5000.0f, .restitution = 1.0f, .isSimulated = false, .useGravity = false, .velocity = {0.0f, 0.0f}};
            Collider c{
                .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{200.0f, 5.0f}),
                .collider_id = CollisionTag_Floor
            };
            scene.AddComponent<Transform>(e, t);
            scene.AddComponent<Render>(e, r);
            scene.AddComponent<PhysicsBody>(e, pb);
            scene.AddComponent<Collider>(e, c);
        }

        for (int i = 0; i < BALL_COUNT; ++i)
        {
            Entity e = scene.CreateEntity();
            int rx = rand() % 30 + 25;
            int ry = rand() % 10;
            int rv = rand() % 20 - 10;
            Transform t{{rx, ry}};
            Render r{RenderType_Char, RenderChar{'o'}};
            PhysicsBody pb{.mass = BALL_MASS, .restitution = RESTITUTION, .useGravity = true, .velocity = {rv, 0.0f}};
            Collider c{
                .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
                .collider_id = CollisionTag_Ball
            };
            scene.AddComponent<Transform>(e, t);
            scene.AddComponent<Render>(e, r);
            scene.AddComponent<PhysicsBody>(e, pb);
            scene.AddComponent<Collider>(e, c);
        }

        scene.Run();
    }

    return 0;
}
