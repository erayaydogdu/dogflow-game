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

#define RUN_SPEED_MIN 100
#define RUN_SPEED_MAX 400
#define TIME_BETWEEN_DOGS 1.0f
#define INITIAL_LIVES 3
#define POINT_PER_DOG 100

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

float _nextDogTimer;
int _score;
int _lives;


void GameInit(void);
void UpdateDrawFrame(void);
void GameEnd(void);

void UnsetDogAt(int);
void SetDogAt(int, Vector2, float);

void DrawDog(void);
void DrawBone(void);
void DrawGameOver(void);

void SpawnDog(void);
void UpdateDogs(void);

Vector2 GetBonePosition(void);

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
	_nextDogTimer = TIME_BETWEEN_DOGS;

	_lives = INITIAL_LIVES;
	_score = 0;

	for (int i = 0; i < DOG_MAX_COUNT; i++)
	{
		UnsetDogAt(i);
	}
}

void GameEnd(void)
{
	_state = END;
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
	Vector2 position = GetBonePosition();
	position.x -= BONE_SOURCE_WIDTH / 2;
	position.y -= BONE_SOURCE_HEIGHT / 2;

	DrawTextureRec(_atlasBone, BONE_SOURCE_RECTANGLE, position,WHITE);
}

void DrawGameOver(void)
{
	const char* gameOverText = TextFormat("Game Over!");
	const char* pressRtoRestartText = TextFormat("Pres R to restart..");

	DrawRectangle(0,0,screenWidth,screenHeight, Fade(WHITE,0.8f));
	DrawText(gameOverText, screenWidth / 2 - MeasureText(gameOverText,40) / 2,screenHeight /2 - 10,40,DARKGRAY);
	DrawText(pressRtoRestartText, screenWidth / 2 - MeasureText(pressRtoRestartText,20) / 2,screenHeight * 0.75f - 10,20,DARKGRAY);

	const char* scoreText = TextFormat("SCORE: %d", _score);
	DrawText(scoreText, screenWidth / 2 - MeasureText(scoreText,40) / 2,screenHeight / 2 - 60,40,DARKGRAY);

	int minutes = (int) (_timeGameEnded -  _timeGameStarted) / 60;
	int seconds = (int) (_timeGameEnded - _timeGameStarted) % 60;
	DrawText(TextFormat("Time played: %d minutes, %d seconds.", minutes, seconds), 20, screenHeight - 40, 20, DARKGRAY);
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
			
			//calculate the points etc
			_lives--;
			if(_lives <=0)
			{
				GameEnd();
				return;
			}
			
			continue;
		}

		Vector2 bonePosition = GetBonePosition();
		if(fabsf(_dogs[i].position.x - bonePosition.x) < BONE_SOURCE_WIDTH / 2 &&
			fabsf(_dogs[i].position.y - bonePosition.y) < BONE_SOURCE_HEIGHT / 2)
		{
			UnsetDogAt(i);
			//get point or score etc
			_score += POINT_PER_DOG;
			continue;
		}

		float offset = _dogs[i].runningSpeed * GetFrameTime();
		_dogs[i].position.y += offset;
	}
}

Vector2 GetBonePosition()
{
    return (Vector2){GetMousePosition().x, screenHeight - BONE_SOURCE_HEIGHT * 2};
}

void UpdateDrawFrame(void)
{
	if (_state == END && IsKeyPressed(KEY_R))
	{
		GameInit();
	}
	//TODO: Add pause mod

	// if (win condition)
	// {
	// 	GameEnd
	// }

	if (_state == PLAYING)
	{
		UpdateDogs();
		_nextDogTimer -= GetFrameTime();
		if (_nextDogTimer < 0)
		{
			_nextDogTimer = TIME_BETWEEN_DOGS;
			SpawnDog();
		}
	}
	
	

	

	BeginDrawing();
		ClearBackground(RAYWHITE);

		// Draw Game
		DrawDog();
		DrawBone();
		if(_state == END)
		{
			DrawGameOver();
		}
		else
		{
			//TODO: Gametime
			const char* scoreText = TextFormat("SCORE: %d", _score);
			const char* livesText = TextFormat("LIVES: %d", _lives);
			DrawText(scoreText, 0,0,32,BLACK);
			DrawText(livesText, 0,48,24,DARKGRAY);
		}
	EndDrawing();

}