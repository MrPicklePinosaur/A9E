
#include <string>
#include "score.h"

PlayerScoreSystem::PlayerScoreSystem(Scene& scene): System{scene}
{
    r = scene.GetRenderer();
}

void
PlayerScoreSystem::OnUpdate()
{
    for (auto& e : scene.MakeEntityView<PlayerScore>()) {
        PlayerScore& player_score = scene.GetComponent<PlayerScore>(e);

        r->WriteStatus(std::string("score: ")+std::to_string(player_score.score), 1);

        break; // only expect one entity with PlayerScore (this is sorta ugly)
    }
}

