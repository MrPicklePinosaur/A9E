
#include "../spawner.h"
#include "pipemaker.h"

using namespace std::chrono;

void
PipeMakerSystem::StartSpawns(std::chrono::milliseconds gap)
{
    this->gap = gap;
    start_spawning = true;
}

void
PipeMakerSystem::OnUpdate()
{
    if (!start_spawning) return;

    steady_clock::time_point now = steady_clock::now();

    if (duration_cast<milliseconds>(now-last_pipe_spawn) > gap) {
        last_pipe_spawn = now;
        SpawnPipe(scene);
    }
}
