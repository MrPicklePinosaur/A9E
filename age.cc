
#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>

#include "config.h"
#include "renderer.h"
#include "inputer.h"
#include "ecs.h"
#include "components/transform.h"
#include "components/physicsbody.h"
#include "components/render.h"
#include "components/ai.h"
#include "components/collider.h"
#include "components/playercontroller.h"

void
contactCallback(Entity a, Entity b)
{
    /* std::cout << "COLLISION BETWEEN " << a << " AND " << b << std::endl; */
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
        Render r{RenderType_Bitmap, bitmap1};
        PhysicsBody pb{.mass = 5.0f, .useGravity = false, .velocity = {0.0f, -5.0f}};
        PlayerController ps{.speed = 1.0f};
        Collider c{.data = std::make_shared<SphereColData>(1.0f, vec2::zero())};
        scene.AddComponent<Transform>(e, t);
        scene.AddComponent<Render>(e, r);
        scene.AddComponent<PhysicsBody>(e, pb);
        scene.AddComponent<PlayerController>(e, ps);
        scene.AddComponent<Collider>(e, c);
    }
    {
        Entity e = scene.CreateEntity();
        Transform t{{2, 10}};
        Render r{RenderType_Bitmap, bitmap2};
        PhysicsBody pb{.mass = 5.0f, .useGravity = false, .velocity = {0.0f, 5.0f}};
        Collider c{.data = std::make_shared<SphereColData>(1.0f, vec2::zero())};
        scene.AddComponent<Transform>(e, t);
        scene.AddComponent<Render>(e, r);
        scene.AddComponent<PhysicsBody>(e, pb);
        scene.AddComponent<Collider>(e, c);
    }

    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(TIME_STEP*1000.0)));
        playercontroller_system.Update();
        collision_system.Update();
        physics_system.Update();
        render_system.Update(); 

        inputer->ClearKeyMap();
    }

    return 0;
}
