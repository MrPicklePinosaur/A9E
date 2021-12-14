#ifndef __SCORE_H__
#define __SCORE_H__

#include "a9e.h"

struct Score {
    int score = 0;
};

class ScoreSystem final : public System
{
    Renderer* r;
public:
    ScoreSystem(Scene& scene);
    ~ScoreSystem() {}
    void BeforeUpdate() override {}
    void OnUpdate() override;
    void AfterUpdate() override {}
};

#endif // __SCORE_H__
