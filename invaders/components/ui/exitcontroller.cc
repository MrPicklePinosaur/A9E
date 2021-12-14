
#include "exitcontroller.h"

ExitControllerSystem::ExitControllerSystem(Scene& scene) : System{scene}
{
    i = scene.GetInputer();
}

void
ExitControllerSystem::OnUpdate()
{
    if (i->GetKeyDown('q')) scene.TerminateScene();
}
