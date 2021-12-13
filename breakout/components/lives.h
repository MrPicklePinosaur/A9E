#ifndef __LIVES_H__
#define __LIVES_H__

#include "a9e.h"

struct Lives {
    int lives_left = 3;
};

class LivesSystem final : public System
{
public:
    LivesSystem(Scene& scene): System{scene} {}
    ~LivesSystem() {}
    void BeforeUpdate() override {}
    void OnUpdate() override;
    void AfterUpdate() override {}
};

#endif // __LIVES_H__
