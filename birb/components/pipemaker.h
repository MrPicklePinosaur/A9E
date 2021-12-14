#ifndef __PIPEMAKER_H__
#define __PIPEMAKER_H__

#include <chrono>
#include "a9e.h"

class PipeMakerSystem final : public System
{
    std::chrono::milliseconds gap; // time between pipes 

    std::chrono::steady_clock::time_point last_pipe_spawn;
    bool start_spawning = false;
public:
    PipeMakerSystem(Scene& scene): System{scene} {}
    ~PipeMakerSystem() {}
    void StartSpawns(std::chrono::milliseconds gap);
    void BeforeUpdate() override {}
    void OnUpdate() override;
    void AfterUpdate() override {}
};

#endif // __PIPEMAKER_H__
