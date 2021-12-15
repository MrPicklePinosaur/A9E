
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
        if (player_hp.invincible) r->WriteStatus(std::string("health: invincible"), 0);
        else if (player_hp.hp >= 0) r->WriteStatus(std::string("health: ")+std::string(player_hp.hp, '+'), 0);

        if (player_hp.hp == 0 && !player_hp.invincible) {
            // lose event
            r->WriteStatus("You are dead.", 0);
            scene.DestroyEntity(e);
        }

        break;
    }
}
