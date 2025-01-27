#include "score.h"
#include "raylib.h"

#include "defines.h"

int _score;
int _lives;

int GetScore(void)
{
    return _score;
}
int GetLives(void)
{
    return _lives;
}
void AddScore(int score)
{
    _score += score;
}
void AddLives(int live)
{
    _lives += live;
}

bool RemoveLives(int live)
{
    _lives -= live;

    return _lives > 0;
}

void ResetScore(void)
{
    _lives = INITIAL_LIVES;
    _score = 0;
}