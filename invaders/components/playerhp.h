#ifndef __PLAYERHP_H__
#define __PLAYERHP_H__

#include "a9e.h"

struct PlayerHp {
    unsigned int hp = 3;
};

class PlayerHpSystem : public System
{
    Renderer* r;
public:
    PlayerHpSystem(Scene& scene);
    ~PlayerHpSystem() {}
    void BeforeUpdate() override {}
    void OnUpdate() override;
    void AfterUpdate() override {}
};

#endif // __PLAYERHP_H__
