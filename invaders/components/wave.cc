
#include <cstdlib>
#include "wave.h"

using namespace std::chrono;

void
WaveSystem::OnUpdate()
{
    if (waves.size() == 0) return;

    Wave& current_wave = waves.front();
    steady_clock::time_point now = steady_clock::now();
    
    // start new wave (if no spawns remaining)
    if (wave_remaining == 0 && duration_cast<seconds>(now-last_wave_time) > current_wave.pad) {

        if (waves.size() > 1) waves.pop(); // keep looping the last wave

        last_wave_time = now;

        current_wave = waves.front();
        wave_remaining = current_wave.count;
    }

    // spawn enemy
    if (wave_remaining > 0 && duration_cast<seconds>(now-last_spawn_time) > current_wave.stagger) {
        last_spawn_time = now;

        // choose random enemy
        if (current_wave.spawns.size() == 0) return;

        unsigned int random_index = rand() % current_wave.spawns.size();
        current_wave.spawns[random_index](scene, current_wave.spawn_point, current_wave.direction);

        --wave_remaining;
    }
}

void
WaveSystem::AddWaves(const std::vector<Wave>& new_waves)
{
    for (auto& wave : new_waves) waves.push(wave);
}

