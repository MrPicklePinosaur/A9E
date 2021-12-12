#ifndef __SPAWNER_H__
#define __SPAWNER_H__

#include <functional>
#include "a9e.h"

using SpawnFunction = std::function<void(Scene&, const vec2&, const vec2&)>;

void SpawnPlayer(Scene& scene, const vec2& pos, const vec2& dir);
void SpawnPlayerBullet(Scene& scene, const vec2& pos, const vec2& dir = vec2{0.0f, -1.0f});

void SpawnBasicEnemy(Scene& scene, const vec2& pos, const vec2& dir);
void SpawnBasicEnemyBullet(Scene& scene, const vec2& pos, const vec2& dir = vec2{0.0f, 1.0f});

void SpawnTwinGunnerEnemy(Scene& scene, const vec2& pos, const vec2& dir);

void SpawnBomberEnemy(Scene& scene, const vec2& pos, const vec2& dir);
void SpawnBomberEnemyBullet(Scene& scene, const vec2& pos, const vec2& dir = vec2{0.0f, 1.0f});

#endif // __SPAWNER_H__
