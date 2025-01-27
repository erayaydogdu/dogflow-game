#include "bone.h"
#include "score.h"
#include "bed.h"
#include "defines.h"

#include <math.h>

extern Texture2D _atlasBone;

void UnsetBone(Bone* bone)
{
    bone->_active = false;
}
void DrawBone(Bone* bone)
{
    if (!bone->_active) return;

    Vector2 positionBone = bone->position;
    positionBone.x -= DOG_SOURCE_WIDTH / 2;
    positionBone.y -= DOG_SOURCE_HEIGHT / 2;

    Rectangle sourceDogBone = (Rectangle){0,0,BONE_SOURCE_WIDTH,BONE_SOURCE_HEIGHT};
    Rectangle destDogBone = (Rectangle){positionBone.x,positionBone.y,BONE_SOURCE_WIDTH / 2,BONE_SOURCE_HEIGHT / 2};

    DrawTexturePro(_atlasBone, sourceDogBone, destDogBone, (Vector2){0, 0},0, WHITE);
}

void SpawnBone(Bone* bone)
{
    int velocity = GetRandomValue(RUN_SPEED_MIN,RUN_SPEED_MAX);
	int posX = GetRandomValue(DOG_SOURCE_WIDTH / 2, SCREEN_WIDTH - (DOG_SOURCE_WIDTH / 2));

    bone->position = (Vector2){posX, -DOG_SOURCE_HEIGHT};
	bone->velocity = velocity;
	bone->_active = true;
	bone->bonusLife = 1;
}
void UpdateBone(Bone* bone)
{
    if(!bone->_active) return;

    Vector2 bedPosition = GetBedPosition();
    if(fabsf(bone->position.x - bedPosition.x) < BED_SOURCE_WIDTH / 2 &&
        fabsf(bone->position.y - bedPosition.y) < BED_SOURCE_HEIGHT / 3)
    {
        UnsetBone(bone);
        //get point or score etc
        AddLives(bone->bonusLife);
    }

    float offset = bone->velocity * GetFrameTime();
    bone->position.y += offset;
}