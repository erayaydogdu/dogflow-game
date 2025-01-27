#include "bed.h"
#include "defines.h"
#include "gamestate.h"

extern Texture2D _atlasBed;

float _bedXPosition;

Vector2 GetBedPosition()
{
	if(GetGameState() == PLAYING)
	{
		_bedXPosition = GetMousePosition().x;
	}
    return (Vector2){_bedXPosition, SCREEN_HEIGHT - BED_SOURCE_HEIGHT /2};
}

void DrawBed(void)
{
	Vector2 position = GetBedPosition();
	position.x -= BED_SOURCE_WIDTH / 2;
	position.y -= BED_SOURCE_HEIGHT / 2;

	DrawTextureRec(_atlasBed, BED_SOURCE_RECTANGLE, position,WHITE);
}
