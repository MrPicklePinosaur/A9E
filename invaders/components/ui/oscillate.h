#ifndef __OSCILLATE_H__
#define __OSCILLATE_H__

#include <chrono>
#include "a9e.h"

struct Oscillate {
    float amplitude;
    float period_scale;
    float axis_x;
};

class OscillateSystem final : public System
{
    std::chrono::steady_clock::time_point start;
public:
    OscillateSystem(Scene& scene);
    ~OscillateSystem() {}
    void BeforeUpdate() override {}
    void OnUpdate() override;
    void AfterUpdate() override {}
};


#endif // __OSCILLATE_H__
