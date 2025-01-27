#ifndef BONE_H_
#define BONE_H_

#include "raylib.h"

typedef struct Bone
{
	int bonusLife;
	Vector2 position;
	int velocity;

	bool _active;
}Bone;

void UnsetBone(Bone*);
void DrawBone(Bone*);
void SpawnBone(Bone*);
void UpdateBone(Bone*);

#endif
