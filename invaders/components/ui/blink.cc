
#include "blink.h"

Blink::Blink(std::chrono::milliseconds blink_time): blink_time{blink_time} {}

void
BlinkSystem::OnUpdate()
{
    using namespace std::chrono;

    for (auto& e : scene.MakeEntityView<Blink,Render>()) {
        Render& render= scene.GetComponent<Render>(e);
        Blink& blink = scene.GetComponent<Blink>(e);

        steady_clock::time_point now = steady_clock::now();
        if (duration_cast<milliseconds>(now-blink.last_blink) > blink.blink_time) {
            blink.last_blink = now;
            render.visible = !render.visible;
        }
    }
}
