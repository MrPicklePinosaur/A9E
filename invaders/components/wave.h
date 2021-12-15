#ifndef __WAVE_H__
#define __WAVE_H__

#include <vector>
#include <queue>
#include <chrono>
#include "a9e.h"

using SpawnFunction = void(*)(Scene&, const vec2&, const vec2&);

struct Wave {
    std::vector<SpawnFunction> spawns;
    int count;     // number of units to spawn in current wave
    std::chrono::milliseconds pad;     // time before next wave
    std::chrono::milliseconds stagger; // time between spawns in current wave
    vec2 spawn_point;
    vec2 direction = vec2{1.0f, 0.0f};
};

class WaveSystem : public System
{
    unsigned int wave_remaining = 0;
    std::queue<Wave> waves;
    std::chrono::steady_clock::time_point last_wave_time;
    std::chrono::steady_clock::time_point last_spawn_time;
public:
    WaveSystem(Scene& scene): System{scene} {}
    ~WaveSystem() {}
    void StartWaves(const std::vector<Wave>& new_waves);
    void BeforeUpdate() override {}
    void OnUpdate() override;
    void AfterUpdate() override {}
private:
    void StartNewWave();
};

#endif // __WAVE_H__
