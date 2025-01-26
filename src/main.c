#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define DOG_SOURCE_WIDTH 32
#define DOG_SOURCE_HEIGHT 32
#define DOG_MAX_COUNT 8
#define DOG_SOURCE_RECTANGLE CLITERAL(Rectangle){0,0,DOG_SOURCE_WIDTH,DOG_SOURCE_HEIGHT}

#define BONE_SOURCE_WIDTH 100
#define BONE_SOURCE_HEIGHT 41
#define BONE_SOURCE_RECTANGLE CLITERAL(Rectangle){0,0,BONE_SOURCE_WIDTH,BONE_SOURCE_HEIGHT}

#define RUN_SPEED_MIN 50
#define RUN_SPEED_MAX 250

const int screenWidth = 600;
const int screenHeight = 600;
const Vector2 screenSize = {screenWidth, screenHeight};

typedef struct Dog
{
	Vector2 position;
	int runningSpeed;

	bool _active;
}Dog;

Dog _dogs[DOG_MAX_COUNT];

typedef enum GameState
{
	PLAYING,
	END
} GameState;

GameState _state;

float _timeGameStarted;
float _timeGameEnded;

Texture2D _atlasDog;
Texture2D _atlasBone;

void GameInit(void);
void UpdateDrawFrame(void);
void GameEnd(void);

void UnsetDogAt(int);
void SetDogAt(int, Vector2, float);
void DrawDog(void);
void DrawBone(void);
void SpawnDog(void);
void UpdateDogs(void);


int main ()
{
	srand(time(0));

	InitWindow(screenWidth, screenHeight, "Sitaroid Game");

	_atlasDog = LoadTexture("resources/dog_alpha.png");
	_atlasBone = LoadTexture("resources/dog_bone.png");

	GameInit();
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		UpdateDrawFrame();
	}

	CloseWindow();
	return 0;
}


void GameInit(void)
{
	_state = PLAYING;
	_timeGameStarted = GetTime();
	for (int i = 0; i < DOG_MAX_COUNT; i++)
	{
		UnsetDogAt(i);
	}
}

void GameEnd(void)
{
	_state = PLAYING;
	_timeGameEnded = GetTime();
}

void UnsetDogAt(int i)
{
	if (i < 0 || i >= DOG_MAX_COUNT) return;
	
	_dogs[i]._active = false;
}

void SetDogAt(int i, Vector2 position, float runSpeed)
{
	if (i < 0 || i >= DOG_MAX_COUNT) return;

	_dogs[i].position = position;
	_dogs[i].runningSpeed = runSpeed;
	_dogs[i]._active = true;
}

void DrawDog(void)
{
	for (int i = 0; i < DOG_MAX_COUNT; i++)
	{
		if(!_dogs[i]._active) continue;

		Vector2 position = _dogs[i].position;
		position.x -= DOG_SOURCE_WIDTH / 2; 
		position.y -= DOG_SOURCE_HEIGHT / 2; 
		DrawTextureRec(_atlasDog, DOG_SOURCE_RECTANGLE, position,WHITE);
	}
	
}

void DrawBone(void)
{
	Vector2 position = {GetMousePosition().x, screenHeight - BONE_SOURCE_HEIGHT * 2};
	position.x -= BONE_SOURCE_WIDTH / 2;
	position.y -= BONE_SOURCE_HEIGHT / 2;

	DrawTextureRec(_atlasBone, BONE_SOURCE_RECTANGLE, position,WHITE);
}

void SpawnDog(void)
{
	int availableIndex = -1;
	for (int i = 0; i < DOG_MAX_COUNT; i++)
	{
		if (!_dogs[i]._active)
		{
			availableIndex = i;
			break;
		}
	}

	if (availableIndex==-1) return;

	int runSpeed = GetRandomValue(RUN_SPEED_MIN,RUN_SPEED_MAX);
	int posX = GetRandomValue(DOG_SOURCE_WIDTH / 2, screenWidth - (DOG_SOURCE_WIDTH / 2));

	SetDogAt(availableIndex,(Vector2){posX, -DOG_SOURCE_HEIGHT}, runSpeed);
	
}

void UpdateDogs(void)
{
	for (int i = 0; i < DOG_MAX_COUNT; i++)
	{
		if(!_dogs[i]._active) continue;

		if(_dogs[i].position.y > screenHeight + DOG_SOURCE_HEIGHT / 2)
		{
			UnsetDogAt(i);
			//TODO: calculate the points etc?
			continue;
		}

		float offset = _dogs[i].runningSpeed * GetFrameTime();
		_dogs[i].position.y += offset;
	}
}

void UpdateDrawFrame(void)
{
	if (_state == END && IsKeyPressed(KEY_R))
	{
		GameInit();
	}
	
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		SpawnDog();
	}

	// if (win condition)
	// {
	// 	GameEnd
	// }
	
	UpdateDogs();
	
	BeginDrawing();
		ClearBackground(RAYWHITE);

		// Draw Game
		DrawDog();
		DrawBone();
		if(_state == END)
		{
			//TODO: Gameover
		}
		else
		{
			//TODO: Gametime
		}
	EndDrawing();

}