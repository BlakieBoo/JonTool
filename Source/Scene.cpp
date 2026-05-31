#include "Scene.hpp"

#include "Editor.hpp"
#include "TextureList.hpp"
#include <raymath.h>
#include <rlgl.h>
#include <algorithm>

static Camera2D camera = { Vector2(0.0f, 0.0f), Vector2(-150.0f, -150.0f), 0.0f, 0.85f };

static enum BoxType
{
	Rect,
	Point,
	Vector
};

static const BoxType CTYPE_Types[] = 
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

static const Color CTYPE_Colors[] =
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

static const BlendMode TTYPE_Blend[] =
{
	BLEND_ALPHA,
	BLEND_ALPHA,
	BLEND_ADDITIVE,
	BLEND_CUSTOM, 
	BLEND_CUSTOM
};

static void DrawBox(JonCollisionRect& box)
{
	Rectangle drawRect = box.collisionRect;
	switch (CTYPE_Types[box.collisionType])
	{
	default:
	case Rect:
		if (drawRect.width < 0.0f)
		{
			drawRect.x += drawRect.width;
			drawRect.width *= -1.0f;
		}
		if (drawRect.height < 0.0f)
		{
			drawRect.y += drawRect.height;
			drawRect.height *= -1.0f;
		}

		DrawRectangle(drawRect.x, drawRect.y, drawRect.width, drawRect.height, ColorAlpha(CTYPE_Colors[box.collisionType], 0.45f));
		DrawRectangleLines(drawRect.x, drawRect.y, drawRect.width, drawRect.height, CTYPE_Colors[box.collisionType]);
		break;
	case Point:
		DrawLine(box.collisionRect.x - 25, box.collisionRect.y, box.collisionRect.x + 25, box.collisionRect.y, CTYPE_Colors[box.collisionType]);
		DrawLine(box.collisionRect.x, box.collisionRect.y - 25, box.collisionRect.x, box.collisionRect.y + 25, CTYPE_Colors[box.collisionType]);
		DrawRectangle(box.collisionRect.x - 10, box.collisionRect.y - 10, 20, 20, ColorAlpha(CTYPE_Colors[box.collisionType], 0.45f));
		break;
	case Vector:
		DrawLine(box.collisionRect.x, box.collisionRect.y, box.collisionRect.width, box.collisionRect.height, CTYPE_Colors[box.collisionType]);
		DrawCircle(box.collisionRect.width, box.collisionRect.height, 5, CTYPE_Colors[box.collisionType]);
		break;
	}
}

static void DrawSprite(JonSpriteRect& sprite, std::string& tex)
{
	Rectangle drawRect = sprite.positionRect;
	Rectangle uvRect = sprite.uvRect;
	if (drawRect.width < 0.0f)
	{
		drawRect.x += drawRect.width;
		drawRect.width *= -1.0f;
		//uvRect.x += uvRect.width;
		uvRect.width *= -1.0f;
	}
	if (drawRect.height < 0.0f)
	{
		drawRect.y += drawRect.height;
		drawRect.height *= -1.0f;
		//uvRect.y += uvRect.height;
		uvRect.height *= -1.0f;
	}
	
	BeginBlendMode(TTYPE_Blend[sprite.transType]);
	if (sprite.transType == TTYPE_DEC)
		rlSetBlendFactors(RL_BLEND_SRC_ALPHA, RL_ONE, RL_FUNC_SUBTRACT);
	else if (sprite.transType == TTYPE_REVERSE)
		rlSetBlendFactors(RL_BLEND_SRC_ALPHA, RL_ONE, RL_FUNC_REVERSE_SUBTRACT);

	if (TexIsLoaded(tex))
		DrawTexturePro(*GetTexture(tex), uvRect, drawRect, Vector2(0.0f, 0.0f), RAD2DEG * sprite.rotZ, ColorAlpha(WHITE, sprite.alpha));
	else
		DrawRectanglePro(drawRect, Vector2(0.0f, 0.0f), RAD2DEG * sprite.rotZ, ColorAlpha(DARKGREEN, sprite.alpha));
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

	BeginBlendMode(BLEND_ALPHA);

	for (JonCollisionRect& box : curJon->collisions)
		DrawBox(box);

	End:
	EndMode2D();
}