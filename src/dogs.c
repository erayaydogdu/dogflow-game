#include "dogs.h"

#include "defines.h"
#include "score.h"
#include "bone.h"
#include "gamestate.h"

#include <math.h>

extern Texture2D _atlasDog;
extern void GameEnd(void);

void UnsetDogAt(Dog* dogs,int i)
{
	if (i < 0 || i >= DOG_MAX_COUNT) return;
	
	dogs[i]._active = false;
}

void SetDogAt(Dog* dogs,int i, Vector2 position, float runSpeed)
{
	if (i < 0 || i >= DOG_MAX_COUNT) return;

	dogs[i].position = position;
	dogs[i].runningSpeed = runSpeed;
	dogs[i]._active = true;
}

void DrawDogs(Dog* dogs)
{
	for (int i = 0; i < DOG_MAX_COUNT; i++)
	{
		if(!dogs[i]._active) continue;

		Vector2 position = dogs[i].position;
		position.x -= DOG_SOURCE_WIDTH / 2; 
		position.y -= DOG_SOURCE_HEIGHT / 2; 
		DrawTextureRec(_atlasDog, DOG_SOURCE_RECTANGLE, position,WHITE);
	}
	
}

void SpawnDog(Dog* dogs)
{
	int availableIndex = -1;
	for (int i = 0; i < DOG_MAX_COUNT; i++)
	{
		if (!dogs[i]._active)
		{
			availableIndex = i;
			break;
		}
	}

	if (availableIndex==-1) return;

	int runSpeed = GetRandomValue(RUN_SPEED_MIN,RUN_SPEED_MAX);
	int posX = GetRandomValue(DOG_SOURCE_WIDTH / 2, SCREEN_WIDTH - (DOG_SOURCE_WIDTH / 2));

	SetDogAt(dogs,availableIndex,(Vector2){posX, -DOG_SOURCE_HEIGHT}, runSpeed);
	
}

void UpdateDogs(Dog* dogs)
{
	for (int i = 0; i < DOG_MAX_COUNT; i++)
	{
		if(!dogs[i]._active) continue;

		if(dogs[i].position.y > SCREEN_HEIGHT + DOG_SOURCE_HEIGHT / 2)
		{
			UnsetDogAt(dogs,i);
			
			//calculate the points etc
			if(!RemoveLives(1))
			{
				GameEnd();
				return;
			}
			
			continue;
		}

		Vector2 bonePosition = GetBonePosition();
		if(fabsf(dogs[i].position.x - bonePosition.x) < BONE_SOURCE_WIDTH / 2 &&
			fabsf(dogs[i].position.y - bonePosition.y) < BONE_SOURCE_HEIGHT / 2)
		{
			UnsetDogAt(dogs,i);
			//get point or score etc
			AddScore(POINT_PER_DOG);
            
			continue;
		}

		float offset = dogs[i].runningSpeed * GetFrameTime();
		dogs[i].position.y += offset;
	}
}