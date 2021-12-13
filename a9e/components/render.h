#ifndef __A9E_RENDER_H__
#define __A9E_RENDER_H__

#include <variant>
#include <vector>
#include <string>
#include "../ecs.h"

enum RenderType {
    RenderType_None = 0,
    RenderType_Char,
    RenderType_Box,
    RenderType_Bitmap,
    RenderType_Text
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

struct RenderText {
    std::string text;
};

struct Render {
    RenderType render_type;
    std::variant<std::monostate, RenderChar, RenderBox, RenderBitmap, RenderText> data;
    bool visible = true;
};

class RenderSystem : public System
{
    Renderer* r;
public:
    RenderSystem(Scene& scene);
    ~RenderSystem();
    void BeforeUpdate() override;
    void OnUpdate() override;
    void AfterUpdate() override;
};

#endif // __A9E_RENDER_H__
