#ifndef __SPAWNER_H__
#define __SPAWNER_H__

#include <functional>
#include "a9e.h"

using SpawnFunction = std::function<void(Scene&, const vec2&)>;

void SpawnPlayer(Scene& scene, const vec2& pos);
void SpawnPlayerBullet(Scene& scene, const vec2& pos);

void SpawnBasicEnemy(Scene& scene, const vec2& pos);
void SpawnBasicEnemyBullet(Scene& scene, const vec2& pos);

void SpawnTwinGunnerEnemy(Scene& scene, const vec2& pos);

#endif // __SPAWNER_H__
