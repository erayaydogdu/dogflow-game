#include "gamestate.h"

GameState _state;

GameState GetGameState()
{
    return _state;
}

void SetGameState(GameState state)
{
    _state = state;
}