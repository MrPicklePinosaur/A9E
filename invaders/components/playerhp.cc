
#include <string>
#include "playerhp.h"

PlayerHpSystem::PlayerHpSystem(Scene& scene): System{scene}
{
    r = scene.GetRenderer();
}

void
PlayerHpSystem::OnUpdate()
{
    for (auto& e : scene.MakeEntityView<PlayerHp>()) {
        PlayerHp& player_hp = scene.GetComponent<PlayerHp>(e);

        // draw health to status (could move this responsibility to other system)
        r->WriteStatus(std::string(player_hp.hp, '+'), 0);

        if (player_hp.hp == 0) {
            // lose event
        }
    }
}
