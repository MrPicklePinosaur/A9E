#ifndef __RENDER_COMPONENT_H__
#define __RENDER_COMPONENT_H__

enum RenderType {
    RenderType_Point = 0,
    RenderType_Bitmap
};

struct Render {
    RenderType render_type;
};

#endif // __RENDER_COMPONENT_H__
