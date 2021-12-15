#ifndef __BLINK_H__
#define __BLINK_H__

#include <chrono>
#include "a9e.h"

struct Blink {
private:
    std::chrono::steady_clock::time_point last_blink;
public:
    std::chrono::milliseconds blink_time;
    Blink() {}
    Blink(std::chrono::milliseconds blink_time);
    friend class BlinkSystem;
};

class BlinkSystem final : public System
{
public:
    BlinkSystem(Scene& scene): System{scene} {}
    ~BlinkSystem() {}
    void BeforeUpdate() override {}
    void OnUpdate() override;
    void AfterUpdate() override {}
};

#endif // __BLINK_H__
