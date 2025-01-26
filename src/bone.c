#include "bone.h"
#include "defines.h"
#include "gamestate.h"

extern Texture2D _atlasBone;

float _boneXPosition;

Vector2 GetBonePosition()
{
	if(GetGameState() == PLAYING)
	{
		_boneXPosition = GetMousePosition().x;
	}
    return (Vector2){_boneXPosition, SCREEN_HEIGHT - BONE_SOURCE_HEIGHT * 2};
}

void DrawBone(void)
{
	Vector2 position = GetBonePosition();
	position.x -= BONE_SOURCE_WIDTH / 2;
	position.y -= BONE_SOURCE_HEIGHT / 2;

	DrawTextureRec(_atlasBone, BONE_SOURCE_RECTANGLE, position,WHITE);
}
