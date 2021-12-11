#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "a9e.h"

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

#endif // __GLOBAL_H__
