#ifndef __SPAWNER_H__
#define __SPAWNER_H__

#include "a9e.h"
#include "components/wave.h"

void SpawnPlayer(Scene& scene, const vec2& pos, const vec2& dir, bool invincible = false);
void SpawnPlayerBullet(Scene& scene, const vec2& pos, const vec2& dir = vec2{0.0f, -1.0f});

void SpawnBasicEnemy(Scene& scene, const vec2& pos, const vec2& dir);
void SpawnBasicEnemyBullet(Scene& scene, const vec2& pos, const vec2& dir = vec2{0.0f, 1.0f});

void SpawnTwinGunnerEnemy(Scene& scene, const vec2& pos, const vec2& dir);
void SpawnMachineGunnerEnemy(Scene& scene, const vec2& pos, const vec2& dir);

void SpawnBomberEnemy(Scene& scene, const vec2& pos, const vec2& dir);
void SpawnBomberEnemyBullet(Scene& scene, const vec2& pos, const vec2& dir = vec2{0.0f, 1.0f});

void SpawnChargerEnemy(Scene& scene, const vec2& pos, const vec2& dir = vec2{0.0f, 1.0f});
void SpawnChargerEnemyBullet(Scene& scene, const vec2& pos, const vec2& dir = vec2{0.0f, 1.0f});

void SpawnStarfishEnemy(Scene& scene, const vec2& pos, const vec2& dir);
void SpawnStarfishEnemyBullet(Scene& scene, const vec2& pos, const vec2& dir);

std::vector<Wave> MakeWaves();

#endif // __SPAWNER_H__
