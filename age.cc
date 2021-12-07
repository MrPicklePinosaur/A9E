
#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>

#include "config.h"
#include "renderer.h"
#include "input.h"
#include "ecs.h"
#include "components/transform.h"
#include "components/physicsbody.h"
#include "components/render.h"
#include "components/ai.h"
#include "components/collider.h"

void
contactCallback(Entity a, Entity b)
{
    /* std::cout << "COLLISION BETWEEN " << a << " AND " << b << std::endl; */
}

int
main(int argc, char** argv)
{
    Scene scene;
    CursesInput input;

    RendererSystem render_system{scene};
    PhysicsSystem physics_system{scene};
    ColliderSystem collision_system{scene};
    collision_system.SetContactCallback(contactCallback);

    RenderBitmap bitmap{
        {{'A',0,0},{'B',1,0},{'C',0,1},{'D',1,1}}
    };

    {
        Entity e = scene.CreateEntity();
        Transform t{{2, 20}};
        Render r{RenderType_Bitmap, bitmap};
        PhysicsBody pb{.mass = 10.0f, .useGravity = false};
        Collider c{.data = std::make_shared<SphereColData>(1.0f, vec2::zero())};
        scene.AddComponent<Transform>(e, t);
        scene.AddComponent<Render>(e, r);
        scene.AddComponent<PhysicsBody>(e, pb);
        scene.AddComponent<Collider>(e, c);
    }
    {
        Entity e = scene.CreateEntity();
        Transform t{{2.5f, 1}};
        Render r{RenderType_Bitmap, bitmap};
        PhysicsBody pb{.mass = 10.0f, .useGravity = false, .velocity = {0.0f, 0.3f}};
        Collider c{.data = std::make_shared<SphereColData>(1.0f, vec2::zero())};
        scene.AddComponent<Transform>(e, t);
        scene.AddComponent<Render>(e, r);
        scene.AddComponent<PhysicsBody>(e, pb);
        scene.AddComponent<Collider>(e, c);
    }

    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(TIME_STEP*1000)));
        render_system.Update(); 
        physics_system.Update();
        collision_system.Update();
    }

    return 0;
}
