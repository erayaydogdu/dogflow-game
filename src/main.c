#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "defines.h"
#include "dogs.h"
#include "bed.h"
#include "bone.h"
#include "score.h"
#include "gamestate.h"

Dog _dogs[DOG_MAX_COUNT];
Bone _bone;
float _timeGameStarted;
float _timeGameEnded;

//Texture2D _atlasDog;
Texture2D _atlasBed;
Texture2D _atlasBone;
Texture2D _atlasDogSprite;

float _nextDogTimer;


void GameInit(Dog*, Bone*);
void UpdateDrawFrame(void);
void GameEnd(void);

void DrawGameOver(void);

int main ()
{
	srand(time(0));
	
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sitaroid Game");
	
	//_atlasDog = LoadTexture("resources/dog_alpha.png");
	_atlasBone = LoadTexture("resources/dog_bone.png");
	_atlasBed = LoadTexture("resources/dog_bed.png");
	_atlasDogSprite = LoadTexture("resources/dogs_sprite.png");

	GameInit(_dogs,&_bone);
	
	// game loop
	while (!WindowShouldClose()) // run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		UpdateDrawFrame();
	}

	//UnloadTexture(_atlasDog);
	UnloadTexture(_atlasBed);
	UnloadTexture(_atlasBone);
	UnloadTexture(_atlasDogSprite);

	CloseWindow();
	return 0;
}


void GameInit(Dog* dogs, Bone* bone)
{
	HideCursor();

	SetGameState(PLAYING);
	_timeGameStarted = GetTime();
	_nextDogTimer = TIME_BETWEEN_DOGS;

	ResetScore();
	UnsetBone(bone);
	for (int i = 0; i < DOG_MAX_COUNT; i++)
	{
		UnsetDogAt(dogs,i);
	}

	
}

void GameEnd(void)
{
	ShowCursor();
	SetGameState(END);
	_timeGameEnded = GetTime();
}

void DrawGameOver(void)
{
	const char* gameOverText = TextFormat("Game Over!");
	const char* pressRtoRestartText = TextFormat("Pres R to restart..");

	DrawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT, Fade(WHITE,0.8f));
	DrawText(gameOverText, SCREEN_WIDTH / 2 - MeasureText(gameOverText,40) / 2,SCREEN_HEIGHT /2 - 10,40,DARKGRAY);
	DrawText(pressRtoRestartText, SCREEN_WIDTH / 2 - MeasureText(pressRtoRestartText,20) / 2,SCREEN_HEIGHT * 0.75f - 10,20,DARKGRAY);

	const char* scoreText = TextFormat("SCORE: %d", GetScore());
	DrawText(scoreText, SCREEN_WIDTH / 2 - MeasureText(scoreText,40) / 2,SCREEN_HEIGHT / 2 - 60,40,DARKGRAY);

	int minutes = (int) (_timeGameEnded -  _timeGameStarted) / 60;
	int seconds = (int) (_timeGameEnded - _timeGameStarted) % 60;
	DrawText(TextFormat("Time played: %d minutes, %d seconds.", minutes, seconds), 20, SCREEN_HEIGHT - 40, 20, DARKGRAY);
}

void UpdateDrawFrame(void)
{
	if (GetGameState() == END && IsKeyPressed(KEY_R))
	{
		GameInit(_dogs, &_bone);
	}
	//TODO: Add pause mod

	// if (win condition)
	// {
	// 	GameEnd
	// }

	if (GetGameState() == PLAYING)
	{
		UpdateBone(&_bone);
		UpdateDogs(_dogs);
		_nextDogTimer -= GetFrameTime();
		if (_nextDogTimer < 0)
		{
			_nextDogTimer = TIME_BETWEEN_DOGS;
			SpawnDog(_dogs);
		}

		//Calculate score and every 10xPOINT_PER_DOG, spawn a bone
		int currentScore = GetScore();
		if (currentScore > 0 && currentScore % (10 * POINT_PER_DOG) == 0)
		{
			SpawnBone(&_bone);
		}
	}
	
	BeginDrawing();
		ClearBackground(RAYWHITE);

		// Draw Game
		DrawDogs(_dogs);
		DrawBone(&_bone);
		DrawBed();
		if(GetGameState() == END)
		{
			DrawGameOver();
		}
		else
		{
			//TODO: Gametime
			const char* scoreText = TextFormat("SCORE: %d", GetScore());
			const char* livesText = TextFormat("LIVES: %d", GetLives());
			DrawText(scoreText, 0,0,32,BLACK);
			DrawText(livesText, 0,48,24,DARKGRAY);
		}
	EndDrawing();

}