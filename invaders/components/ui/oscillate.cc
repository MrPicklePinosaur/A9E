
#include <chrono>
#include <cmath>
#include "oscillate.h"

OscillateSystem::OscillateSystem(Scene& scene): System{scene}, start{std::chrono::steady_clock::now()} {}

void
OscillateSystem::OnUpdate()
{
    using namespace std::chrono;

    for (auto& e : scene.MakeEntityView<Transform,Oscillate>()) {
        Transform& transform = scene.GetComponent<Transform>(e);
        Oscillate& oscillate = scene.GetComponent<Oscillate>(e);

        steady_clock::time_point now = steady_clock::now();
        transform.pos.y = oscillate.axis_x+oscillate.amplitude*sin(oscillate.period_scale*duration_cast<milliseconds>(now-start).count()/1000.0f);
        
    }

}
