#ifndef DOG_H_
#define DOG_H_

#include "raylib.h"

typedef struct Dog
{
	Vector2 position;
	int runningSpeed;

	bool _active;
}Dog;

void UnsetDogAt(Dog*, int);
void SetDogAt(Dog*,int, Vector2, float);
void DrawDogs(Dog*);
void SpawnDog(Dog*);
void UpdateDogs(Dog*);

#endif