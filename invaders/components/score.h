#ifndef __SCORE_H__
#define __SCORE_H__

#include "a9e.h"

struct ScoreReward {
    unsigned int reward = 0;
};

struct PlayerScore {
    unsigned int score = 0;
};

class PlayerScoreSystem final : public System
{
    Renderer* r;
public:
    PlayerScoreSystem(Scene& scene);
    ~PlayerScoreSystem() {}
    void BeforeUpdate() override {}
    void OnUpdate() override;
    void AfterUpdate() override {}
};


#endif // __SCORE_H__
