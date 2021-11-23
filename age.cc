
#include <cstdio>
#include <iostream>

#include "renderer.h"
#include "ecs.h"
#include "components/transform.h"

int
main(int argc, char** argv)
{
    /* CursesRenderer(); */
    ComponentArray<Transform> ca;

    for (int i = 0; i < 100; ++i) {
        Transform t{{i, i}};
        ca.AddComponent(i, t);
    }

    for (auto it = ca.begin(); it != ca.end(); ++it)
        std::cout << (*it).pos.x << "," << (*it).pos.y << std::endl;

    return 0;
}
