
#include "transform.h"
#include "render.h"

RenderSystem::RenderSystem(Scene& scene): System{scene}
{
    r = scene.GetRenderer();
}

RenderSystem::~RenderSystem() {}

void RenderSystem::BeforeUpdate()
{
    r->ClearGameScreen();     
}

void
RenderSystem::OnUpdate()
{
    for (auto& e : scene.MakeEntityView<Transform,Render>()) {
        Transform& transform = scene.GetComponent<Transform>(e);
        Render& render = scene.GetComponent<Render>(e);

        if (!render.visible) continue;

        switch (render.render_type) {
            case RenderType_Char: {
                RenderChar data = std::get<RenderChar>(render.data);
                r->DrawChar(data.c, transform.pos.x, transform.pos.y);
                break;
            }
            case RenderType_Box: {
                RenderBox data = std::get<RenderBox>(render.data);
                r->DrawBox(data.c, transform.pos.x, transform.pos.y, data.w, data.h);
                break;
            }
            case RenderType_Bitmap: {
                RenderBitmap data = std::get<RenderBitmap>(render.data);
                // TODO this can be optimized (convert bitmap to list of strings and draw the entire line)
                for (auto& pixel : data.pixels)
                    r->DrawChar(pixel.c, transform.pos.x+pixel.x, transform.pos.y+pixel.y);
                break;
            }
            case RenderType_Text: {
                RenderText data = std::get<RenderText>(render.data);
                r->DrawText(data.text, transform.pos.x, transform.pos.y);
                break;
            }
            default: { }
        }
    }
}

void RenderSystem::AfterUpdate()
{
    r->DrawGameScreen();
    r->RefreshGameScreen();
    r->DrawStatusScreen();
    r->RefreshStatusScreen();
}

