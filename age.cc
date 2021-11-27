
#include <cstdio>
#include <iostream>

#include "renderer.h"
#include "ecs.h"
#include "components/transform.h"
#include "components/render.h"

int
main(int argc, char** argv)
{
    Scene scene;

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

#if 0
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
