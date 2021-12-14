
#include "menucontroller.h"

void
MenuControllerSystem::OnUpdate()
{
    Inputer* inputer = scene.GetInputer();
    if (inputer->GetKeyDown(' ')) scene.TerminateScene();
}

