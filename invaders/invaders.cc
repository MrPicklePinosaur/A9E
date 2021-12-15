
#include <vector>
#include <string>
#include <chrono>

#include "a9e.h"
#include "components/ui/menucontroller.h"
#include "components/ui/exitcontroller.h"
#include "components/ui/blink.h"
#include "components/playercontroller.h"
#include "components/enemycontroller.h"
#include "components/playerhp.h"
#include "components/enemyhp.h"
#include "components/score.h"
#include "components/wave.h"
#include "spawner.h"
#include "common.h"

void
InitRenderer(CursesRenderer& r)
{
    enum ColorPair {
        ColorPair_Player = 1,
        ColorPair_BasicEnemyBullet,
        ColorPair_StartText,
        ColorPair_LogoText,
    };

    // setup colors pairs
    r.RegisterColorPair(ColorPair_Player, CursesRenderer::BLUE, CursesRenderer::BLACK);
    r.RegisterStyle(RenderStyle_Player, {ColorPair_Player, CursesRenderer::BOLD});

    r.RegisterColorPair(ColorPair_BasicEnemyBullet, CursesRenderer::RED, CursesRenderer::BLACK);
    r.RegisterStyle(RenderStyle_BasicEnemyBullet, {ColorPair_BasicEnemyBullet, CursesRenderer::NORMAL});

    r.RegisterColorPair(ColorPair_StartText, CursesRenderer::YELLOW, CursesRenderer::BLACK);
    r.RegisterStyle(RenderStyle_StartText, {ColorPair_StartText, CursesRenderer::BOLD});

    r.RegisterColorPair(ColorPair_LogoText, CursesRenderer::CYAN, CursesRenderer::BLACK);
    r.RegisterStyle(RenderStyle_LogoText, {ColorPair_LogoText, CursesRenderer::BOLD});

}

int
main(int argc, char** argv)
{
    bool enable_color = false;
    if (argc >= 2 && std::string("-c").compare(std::string(argv[1])) == 0) enable_color = true;

    CursesRenderer renderer{enable_color};
    CursesInputer inputer{};

    InitRenderer(renderer);

    { // menu scene

        Scene scene{&renderer, &inputer};

        scene.RegisterSystem<RenderSystem>();
        scene.RegisterSystem<MenuControllerSystem>();
        scene.RegisterSystem<BlinkSystem>();

        std::vector<std::string> logo = {
            " _                     _                ",
            "(_)_ ____   ____ _  __| | ___ _ __ ___  ",
            "| | '_ \\ \\ / / _` |/ _` |/ _ \\ '__/ __| ",
            "| | | | \\ V / (_| | (_| |  __/ |  \\__ \\ ",
            "|_|_| |_|\\_/ \\__,_|\\__,_|\\___|_|  |___/ ",
            "                Created using A9E       "
        };
        for (int i = 0; i < logo.size(); ++i) {
            Entity e = scene.CreateEntity();
            scene.AddComponent<Transform>(e, {.pos = {20, 6+i}});
            scene.AddComponent<Render>(e, {RenderType_Text, RenderText{logo[i], RenderStyle_LogoText}});
        }
 
        {
            using namespace std::chrono_literals;
            Entity e = scene.CreateEntity();
            scene.AddComponent<Transform>(e, {.pos = {27, 18}});
            scene.AddComponent<Render>(e, {RenderType_Text, RenderText{"[ Press Space to Begin ]", RenderStyle_StartText}});
            scene.AddComponent<Blink>(e, {1000ms});
        }

        renderer.WriteStatus("Controls:", 0);
        renderer.WriteStatus("[A] + [D] to move left and right", 1);
        renderer.WriteStatus("[SPACE] to shoot", 2);

        scene.Run();
    }
    renderer.ClearStatusScreen();

    { // game scene
        Scene scene{&renderer, &inputer};

        scene.RegisterSystem<RenderSystem>();
        scene.RegisterSystem<ExitControllerSystem>();
        scene.RegisterSystem<PhysicsSystem>();
        auto* collider_system = scene.RegisterSystem<ColliderSystem>();
        scene.RegisterSystem<PlayerControllerSystem>();
        scene.RegisterSystem<PlayerHpSystem>();
        scene.RegisterSystem<EnemyHpSystem>();
        scene.RegisterSystem<PlayerScoreSystem>();
        scene.RegisterSystem<EnemyControllerSystem>();
        auto* wave_system = scene.RegisterSystem<WaveSystem>();

        collider_system->SetCollidesWith(CollisionTag_PlayerBullet, CollisionTag_Enemy);
        collider_system->SetCollidesWith(CollisionTag_EnemyBullet, CollisionTag_Player);
        wave_system->StartWaves(MakeWaves());
        SpawnPlayer(scene, vec2{10, 20}, {1.0f, 0.0f});

        scene.Run();
    }

    return 0;
}

