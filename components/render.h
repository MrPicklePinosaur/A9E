#ifndef __RENDER_COMPONENT_H__
#define __RENDER_COMPONENT_H__

#include "transform.h"
#include "../ecs.h"

enum RenderType {
    RenderType_Char = 0,
    RenderType_Bitmap
};

struct Render {
    RenderType render_type;
};

class RendererSystem : public System
{
    CursesRenderer r;
public:
    RendererSystem(Scene& scene);
    ~RendererSystem();
    void BeforeUpdate() override;
    void OnUpdate() override;
    void AfterUpdate() override;
};

RendererSystem::RendererSystem(Scene& scene): System{scene} {}
RendererSystem::~RendererSystem() {}

void RendererSystem::BeforeUpdate()
{
    r.ClearGameScreen();     
}

void
RendererSystem::OnUpdate()
{
    for (auto& e : scene.MakeEntityView<Transform,Render>()) {
        Transform transform = scene.GetComponent<Transform>(e);
        r.DrawChar('A', transform.pos.x, transform.pos.y);
    }
}

void RendererSystem::AfterUpdate()
{
    r.RenderGameScreen();
}

#endif // __RENDER_COMPONENT_H__
