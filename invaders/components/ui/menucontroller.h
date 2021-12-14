#ifndef __MENUCONTROLLER_H__
#define __MENUCONTROLLER_H__

#include "a9e.h"

class MenuControllerSystem final : public System
{
public:
    MenuControllerSystem(Scene& scene): System{scene} {}
    ~MenuControllerSystem() {}
    void BeforeUpdate() override {}
    void OnUpdate() override;
    void AfterUpdate() override {}
};

#endif // __MENUCONTROLLER_H__
