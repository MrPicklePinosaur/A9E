#ifndef __RENDER_H__
#define __RENDER_H__

#include <variant>
#include <vector>
#include "transform.h"
#include "../ecs.h"

enum RenderType {
    RenderType_None = 0,
    RenderType_Char,
    RenderType_Box,
    RenderType_Bitmap
};

struct RenderChar {
    char c;
};
struct RenderBox {
    char c;
    float w, h;
};

struct BitmapPixel {
    char c;
    int x, y;
};
struct RenderBitmap {
    std::vector<BitmapPixel> pixels;
};

struct Render {
    RenderType render_type;
    std::variant<std::monostate, RenderChar, RenderBox, RenderBitmap> data;
    bool visible = true;
};

class RendererSystem : public System
{
    Renderer* r;
public:
    RendererSystem(Scene& scene);
    ~RendererSystem();
    void BeforeUpdate() override;
    void OnUpdate() override;
    void AfterUpdate() override;
};

RendererSystem::RendererSystem(Scene& scene): System{scene}
{
    r = scene.GetRenderer();
}

RendererSystem::~RendererSystem() {}

void RendererSystem::BeforeUpdate()
{
    r->ClearGameScreen();     
}

void
RendererSystem::OnUpdate()
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
        }
    }
}

void RendererSystem::AfterUpdate()
{
    r->DrawGameScreen();
    r->RefreshGameScreen();
}

#endif // __RENDER_H__
