
#include <cstdlib>
#include "wave.h"

using namespace std::chrono;

void
WaveSystem::OnUpdate()
{
    if (waves.size() == 0) return;

    const Wave& current_wave = waves.front();
    steady_clock::time_point now = steady_clock::now();
    
    // start new wave (if no spawns remaining)
    if (wave_remaining == 0 && duration_cast<milliseconds>(now-last_wave_time) > current_wave.pad) {

        if (waves.size() > 1) waves.pop(); // keep looping the last wave

        last_wave_time = now;
        wave_remaining = waves.front().count;
    }
    // spawn enemy
    else if (wave_remaining > 0 && duration_cast<milliseconds>(now-last_spawn_time) > current_wave.stagger) {
        last_spawn_time = now;

        // choose random enemy
        if (current_wave.spawns.size() > 0) {
            unsigned int random_index = rand() % current_wave.spawns.size();
            current_wave.spawns.at(random_index)(scene, current_wave.spawn_point, current_wave.direction);
        }

        --wave_remaining;
    }
}

void
WaveSystem::StartWaves(const std::vector<Wave>& new_waves)
{
    for (auto& wave : new_waves) waves.push(wave);
    if (waves.size() > 0) wave_remaining = waves.front().count;

    steady_clock::time_point now = steady_clock::now();
    last_wave_time = now;
    last_wave_time = now;
}

