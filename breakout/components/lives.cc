
#include <string>
#include "lives.h"

void
LivesSystem::OnUpdate()
{
    Renderer* renderer = scene.GetRenderer();

    for (auto& e : scene.MakeEntityView<Lives>()) {
        Lives& lives = scene.GetComponent<Lives>(e);

        if (lives.lives_left >= 0) renderer->WriteStatus(std::string(lives.lives_left, '+') , 0);

        if (lives.lives_left == 0) {
            // lose
        }

        break;
    }
}
