#ifndef __SPAWNER_H__
#define __SPAWNER_H__

#include "a9e.h"

void SpawnBall(Scene& scene, vec2 pos);
void SpawnPlayer(Scene& scene, vec2 pos);

void SpawnWalls(Scene& scene);
void SpawnLoseTrigger(Scene& scene);

#endif // __SPAWNER_H__
