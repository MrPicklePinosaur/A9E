
#include "score.h"

ScoreSystem::ScoreSystem(Scene& scene): System{scene}
{
    r = scene.GetRenderer();
}

void
ScoreSystem::OnUpdate()
{
    for (auto& e : scene.MakeEntityView<Score>()) {
        Score& score = scene.GetComponent<Score>(e);

        r->WriteStatus(std::string("score: ")+std::to_string(score.score), 0);

        break;
    }
}
