
#include "animate.h"

Animate::Animate(std::chrono::milliseconds animate_speed, std::vector<Render> frames):
    animate_speed{animate_speed}, frames{frames} {}

void
AnimateSystem::OnUpdate()
{
    using namespace std::chrono;

    for (auto& e : scene.MakeEntityView<Animate>()) {
        Animate& animate = scene.GetComponent<Animate>(e);

        steady_clock::time_point now = steady_clock::now();

        if (duration_cast<milliseconds>(now-animate.last_frame) > animate.animate_speed) {

            if (scene.HasComponent<Render>(e)) scene.RemoveComponent<Render>(e);

            animate.last_frame = now;
            animate.current_frame = (animate.current_frame+1) % animate.frames.size();

            scene.AddComponent<Render>(e, animate.frames.at(animate.current_frame));

        }

    }
}
