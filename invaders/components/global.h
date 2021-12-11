#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <string>
#include "../../a9e/a9e.h"

// this is sorta abuse of the system, maybe refactor

struct GlobalState {
    unsigned int player_hp = 3;
};

class GlobalSystem : public System
{
public:
    GlobalSystem(Scene& scene): System{scene} {}
    ~GlobalSystem() {}
    void BeforeUpdate() override {}
    void OnUpdate() override;
    void AfterUpdate() override {}
};

void
GlobalSystem::OnUpdate()
{
    Renderer* r = scene.GetRenderer();
    GlobalState& g = scene.getGlobal<GlobalState>();

    r->WriteStatus(std::string(g.player_hp, '+'), 0);

}

#endif // __GLOBAL_H__
