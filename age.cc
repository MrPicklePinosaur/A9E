
#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>

#include <cstdlib>

#include "config.h"
#include "renderer.h"
#include "inputer.h"
#include "ecs.h"
#include "components/transform.h"
#include "components/physicsbody.h"
#include "components/render.h"
#include "components/collider.h"
#include "components/playercontroller.h"

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
    Inputer* inputer = scene.GetInputer();

    RendererSystem render_system{scene};
    PhysicsSystem physics_system{scene};
    PlayerControllerSystem playercontroller_system{scene};
    ColliderSystem collision_system{scene};
    collision_system.SetContactCallback(contactCallback);

    RenderBitmap bitmap1{
        {{'A',0,0},{'A',1,0},{'A',0,1},{'A',1,1}}
    };
    RenderBitmap bitmap2{
        {{'B',0,0},{'B',1,0},{'B',0,1},{'B',1,1}}
    };

    {
        Entity e = scene.CreateEntity();
        Transform t{{2, 20}};
        Render r{RenderType_Box, RenderBox{'A', 70, 5}};
        PhysicsBody pb{.mass = 5000.0f, .restitution = 1.0f, .isSimulated = false, .useGravity = false, .velocity = {0.0f, 0.0f}};
        PlayerController ps{.speed = 1.0f};
        /* Collider c{.data = std::make_shared<SphereColData>(1.0f)}; */
        Collider c{.data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{100.0f, 5.0f})};
        scene.AddComponent<Transform>(e, t);
        scene.AddComponent<Render>(e, r);
        scene.AddComponent<PhysicsBody>(e, pb);
        scene.AddComponent<PlayerController>(e, ps);
        scene.AddComponent<Collider>(e, c);
    }
    for (int i = 0; i < 20; ++i)
    {
        Entity e = scene.CreateEntity();
        int rx = rand() % 20 + 30;
        int ry = rand() % 10;
        int rv = rand() % 20 - 10;
        Transform t{{rx, ry}};
        /* Render r{RenderType_Bitmap, bitmap2}; */
        Render r{RenderType_Char, RenderChar{'B'}};
        PhysicsBody pb{.mass = 1.0f, .restitution = 0.9f, .useGravity = true, .velocity = {rv, 0.0f}};
        /* Collider c{.data = std::make_shared<SphereColData>(1.0f)}; */
        Collider c{.data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f})};
        scene.AddComponent<Transform>(e, t);
        scene.AddComponent<Render>(e, r);
        scene.AddComponent<PhysicsBody>(e, pb);
        scene.AddComponent<Collider>(e, c);
    }

    while(true) {
        std::chrono::steady_clock::time_point beg_tick = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(TIME_STEP*1000.0)));
        playercontroller_system.Update();
        collision_system.Update();
        physics_system.Update();
        render_system.Update(); 

        inputer->ClearKeyMap();
        std::chrono::steady_clock::time_point end_tick = std::chrono::steady_clock::now();
        scene.delta = std::chrono::duration_cast<std::chrono::nanoseconds>(end_tick-beg_tick).count()/1000000000.0f;
    }

    return 0;
}
