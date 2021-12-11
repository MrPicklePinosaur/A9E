#ifndef __SPAWNER_H__
#define __SPAWNER_H__

#include <functional>
#include "a9e.h"

using SpawnFunction = std::function<void(Scene&, const vec2&)>;

void SpawnBasicEnemy(Scene& scene, const vec2& pos);
void SpawnPlayerBullet(Scene& scene, const vec2& pos);
void SpawnEnemyBullet(Scene& scene, const vec2& pos);

#endif // __SPAWNER_H__
