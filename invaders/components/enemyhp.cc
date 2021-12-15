
#include "enemyhp.h"

void
EnemyHpSystem::OnUpdate()
{
    for (auto& e : scene.MakeEntityView<EnemyHp>()) {
        EnemyHp& enemy_hp = scene.GetComponent<EnemyHp>(e);

        if (enemy_hp.health <= 0) scene.DestroyEntity(e);        
    }
}

