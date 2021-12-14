#ifndef __EXITCONTROLLER_H__
#define __EXITCONTROLLER_H__

#include "a9e.h"

class ExitControllerSystem final : public System
{
    Inputer* i;
public:
    ExitControllerSystem(Scene& scene);
    ~ExitControllerSystem() {}
    void BeforeUpdate() override {}
    void OnUpdate() override;
    void AfterUpdate() override {}
};

#endif // __EXITCONTROLLER_H__
