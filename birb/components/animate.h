#ifndef __ANIMATE_H__
#define __ANIMATE_H__

#include <vector>
#include <chrono>
#include "a9e.h"

// TODO: maybe dont use chrono - just use regular float here
struct Animate {
private:
    std::chrono::steady_clock::time_point last_frame;
    unsigned int current_frame = 0;
public:
    std::chrono::milliseconds animate_speed; 
    std::vector<Render> frames;
    friend class AnimateSystem;
    Animate() {}
    Animate(std::chrono::milliseconds animate_speed, std::vector<Render> frames);
};

class AnimateSystem final : public System
{
public:
    AnimateSystem(Scene& scene): System{scene} {}
    ~AnimateSystem() {}
    void BeforeUpdate() override {}
    void OnUpdate() override;
    void AfterUpdate() override {}
};

#endif // __ANIMATE_H__
