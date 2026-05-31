#include "Scene.hpp"

#include "Editor.hpp"
#include "TextureList.hpp"
#include <raymath.h>
#include <algorithm>

static Camera2D camera = { Vector2(0.0f, 0.0f), Vector2(-150.0f, -150.0f), 0.0f, 0.75f };

static enum BoxType
{
	Rect,
	Point,
	Vector
};

static BoxType CTYPE_Types[] = 
{
	Rect,
	Rect,
	Point,
	Rect,
	Vector,
	Rect,
	Point,
	Point,
	Point,
	Rect,
	Rect,
	Point,
	Point,
	Point,
	Point,
	Point,
	Point,
	Point
};

static Color CTYPE_Colors[] =
{
	BLUE,
	RED,
	PINK,
	PINK,
	PINK,
	YELLOW,
	YELLOW,
	DARKPURPLE,
	DARKPURPLE,
	PURPLE,
	GREEN,
	DARKPURPLE,
	DARKPURPLE,
	WHITE,
	WHITE,
	WHITE,
	WHITE,
	BROWN
};

static void DrawBox(JonCollisionRect& box)
{
	switch (CTYPE_Types[box.collisionType])
	{
	default:
	case Rect:
		DrawRectangle(-box.collisionRect.x, box.collisionRect.y, -box.collisionRect.width, box.collisionRect.height, ColorAlpha(CTYPE_Colors[box.collisionType], 0.45f));
		DrawRectangleLines(-box.collisionRect.x, box.collisionRect.y, -box.collisionRect.width, box.collisionRect.height, CTYPE_Colors[box.collisionType]);
		break;
	case Point:
		DrawLine(-box.collisionRect.x - 25, box.collisionRect.y, -box.collisionRect.x + 25, box.collisionRect.y, CTYPE_Colors[box.collisionType]);
		DrawLine(-box.collisionRect.x, box.collisionRect.y - 25, -box.collisionRect.x, box.collisionRect.y + 25, CTYPE_Colors[box.collisionType]);
		DrawRectangle(-box.collisionRect.x - 10, box.collisionRect.y - 10, 20, 20, ColorAlpha(CTYPE_Colors[box.collisionType], 0.45f));
		break;
	case Vector:
		DrawLine(-box.collisionRect.x, box.collisionRect.y, -box.collisionRect.width, box.collisionRect.height, CTYPE_Colors[box.collisionType]);
		DrawCircle(-box.collisionRect.width, box.collisionRect.height, 5, CTYPE_Colors[box.collisionType]);
		break;
	}
}

static void DrawSprite(JonSpriteRect& sprite, std::string& tex)
{
	std::cout << TexIsLoaded(tex) << std::endl;
	if (TexIsLoaded(tex))
		DrawTexturePro(*GetTexture(tex), sprite.uvRect, sprite.positionRect, Vector2(0.0f, 0.0f), 0.0f, WHITE);
	else
		DrawRectanglePro(sprite.positionRect, Vector2(0.0f, 0.0f), 0.0f, DARKGREEN);
}

void DrawScene()
{
	camera.offset = Vector2(GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f);

	if (!IsEditorHovered())
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			camera.target = Vector2Add(camera.target, Vector2Multiply(GetMouseDelta(), Vector2(-1.0f, -1.0f)));
			HideCursor();
			DisableCursor();
		}
		else if (IsCursorHidden())
		{
			EnableCursor();
			ShowCursor();
		}
	}

	BeginMode2D(camera);

	DrawLine(-10000, 0, 10000, 0, RAYWHITE);
	DrawLine(0, -10000, 0, 10000, RAYWHITE);

	Jon* curJon = GetCurrentJon();
	if (!curJon)
		goto End;
	
	for (int i = 0; i < curJon->sprites.size(); i++)
		DrawSprite(curJon->sprites[i], curJon->usedTextures[std::clamp(curJon->sprites[i].spriteIndex > 0 ? curJon->sprites[i].spriteIndex : (uint32_t)i,
			0u, (uint32_t)curJon->usedTextures.size() - 1)]);

	for (JonCollisionRect& box : curJon->collisions)
		DrawBox(box);

	End:
	EndMode2D();
}