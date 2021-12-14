
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

void SpawnPipe(Scene& scene)
{
#if 0
    Entity e = scene.CreateEntity();
    scene.AddComponent<Transform>(e, {.pos = pos});
    scene.AddComponent<Render>(e, {RenderType_Box, RenderBox{'#', ,}});
    scene.AddComponent<PhysicsBody>(e, {
        .mass = 1.0f,
        .isSimulated = true,
        .cleanOffScreen = false,
        .velocity = {0.0f, 0.0f};
    });
    scene.AddComponent<Collider>(e, {
        .data = std::make_shared<BoxColData>(vec2{0.0f, 0.0f}, vec2{1.0f, 1.0f}),
        .collider_id = CollisionTag_Pipe
    });
#endif
}

