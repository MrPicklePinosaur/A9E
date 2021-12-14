
#include <cstdlib>
#include "common.h"
#include "components/playercontroller.h"
#include "spawner.h"

void SpawnPlayer(Scene& scene, const vec2& pos)
{
    Entity e = scene.CreateEntity();
    RenderBitmap bm = {{
        {'/',-1,0},{'O',0,0},{'>',1,0}
    }};

    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Bitmap, bm});
    scene.AddComponent<PhysicsBody>(e, {
        .mass = 1.0f,
        .isSimulated = true,
        .useGravity = true,
        .cleanOffScreen = false,
    });
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Player,
        .onCollide = [](Scene& scene, Entity self, Entity other) {
            Collider& col_other = scene.GetComponent<Collider>(other);
            if (col_other.collider_id == CollisionTag_Pipe) scene.DestroyEntity(self);
        }
    });
    scene.AddComponent<PlayerController>(e, {.jump_impulse = 10.0f});
}

const int PIPE_GAP = 5;     // how big the hole in the pipe is
const int PIPE_WIDTH = 4; 
const int PIPE_PADDING = 3; // how close to top or bottom of screen the pipe gap can be
void SpawnPipe(Scene& scene)
{
    Renderer* r = scene.GetRenderer();
    int screen_h = r->GetScreenHeight() - r->GetStatusHeight();
    int screen_w = r->GetScreenWidth();

    int pipe_gap_start = rand() % (screen_h-PIPE_PADDING-PIPE_GAP) + PIPE_PADDING;

    { // top of pipe
        Entity e = scene.CreateEntity();
        scene.AddComponent<Transform>(e, {.pos = {screen_w-PIPE_WIDTH, 0}});
        scene.AddComponent<Render>(e, {RenderType_Box, RenderBox{'#', PIPE_WIDTH, pipe_gap_start}});
        scene.AddComponent<PhysicsBody>(e, {.mass = 1.0f, .isSimulated = true, .velocity = {-10.0f, 0.0f}});
        scene.AddComponent<Collider>(e, {
            .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{PIPE_WIDTH, pipe_gap_start}),
            .collider_id = CollisionTag_Pipe
        });
    }
    { // bottom of pipe
        int pipe_height = screen_h - pipe_gap_start - PIPE_GAP;
        Entity e = scene.CreateEntity();
        scene.AddComponent<Transform>(e, {.pos = {screen_w-PIPE_WIDTH, pipe_gap_start+PIPE_GAP}});
        scene.AddComponent<Render>(e, {RenderType_Box, RenderBox{'#', PIPE_WIDTH, pipe_height}});
        scene.AddComponent<PhysicsBody>(e, {.mass = 1.0f, .isSimulated = true, .velocity = {-10.0f, 0.0f}});
        scene.AddComponent<Collider>(e, {
            .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{PIPE_WIDTH, pipe_height}),
            .collider_id = CollisionTag_Pipe
        });
    }
}

