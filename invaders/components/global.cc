
#include <string>
#include "global.h"

void
GlobalSystem::OnUpdate()
{
    Renderer* r = scene.GetRenderer();
    GlobalState& g = scene.getGlobal<GlobalState>();

    r->WriteStatus(std::string(g.player_hp, '+'), 0);

}
