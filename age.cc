
#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>

#include "config.h"
#include "renderer.h"
#include "ecs.h"
#include "components/transform.h"
#include "components/physicsbody.h"
#include "components/render.h"
#include "components/ai.h"
#include "components/collider.h"

int
main(int argc, char** argv)
{
    Scene scene;

    RendererSystem render_system{scene};
    AISystem ai_system{scene};
    PhysicsSystem physics_system{scene};
    ColliderSystem collision_system{scene};

    RenderBitmap bitmap{
        {{'A',0,0},{'B',1,0},{'C',0,1},{'D',1,1}}
    };

    {
        Entity e = scene.CreateEntity();
        Transform t{{2, 30}};
        Render r{RenderType_Bitmap, bitmap};
        PhysicsBody pb{.useGravity = false};
        Collider c{.data = std::make_shared<SphereColData>(1.0f, vec2::zero())};
        scene.AddComponent<Transform>(e, t);
        scene.AddComponent<Render>(e, r);
        scene.AddComponent<PhysicsBody>(e, pb);
        scene.AddComponent<Collider>(e, c);
    }
    {
        Entity e = scene.CreateEntity();
        Transform t{{2, 1}};
        Render r{RenderType_Bitmap, bitmap};
        PhysicsBody pb;
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
        /* ai_system.Update(); */ 
    }

#if 0

    for (int i = 0; i < 20; ++i) {
        Entity e = scene.CreateEntity();
        if (i % 2 == 1) continue;
        Transform t{{1, 2}};
        scene.AddComponent<Transform>(e, t);
    }

    scene.Debug();

    EntityView v = scene.MakeEntityView<>();
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << std::endl;
    }

    // =-=-=-=-=-=-=

    ComponentManager cm;

    for (int i = 0; i < 10; ++i) {
        Transform t{{i, i}};
        cm.AddComponent(i, t);
    }
    for (int i = 0; i < 10; ++i) {
        Render r{RenderType_Point};
        cm.AddComponent(i, r);
    }

    auto t = cm.GetComponent<Transform>(9);
    std::cout << t.pos.x << "," << t.pos.y << std::endl;

    ComponentArray<Transform> ca;

    for (int i = 0; i < 10; ++i) {
        Transform t{{i, i}};
        ca.AddComponent(i, t);
    }

    for (auto it = ca.begin(); it != ca.end(); ++it)
        std::cout << (*it).pos.x << "," << (*it).pos.y << std::endl;

    std::cout << "=-=-=-=-=-=" << std::endl;
    ca.RemoveComponent(2);

    for (auto it = ca.begin(); it != ca.end(); ++it)
        std::cout << (*it).pos.x << "," << (*it).pos.y << std::endl;
#endif

    return 0;
}
