#pragma once

#include "Common.hpp"

//structs taken from my private bbtag decomp lmao

enum ZTYPE
{
	ZTYPE_DEFAULT = 0,
	ZTYPE_MOST_FAR = 1,
	ZTYPE_FAR = 2,
	ZTYPE_LITTLE_FAR = 3,
	ZTYPE_LITTLE_NEAR = 4,
	ZTYPE_NEAR = 5,
	ZTYPE_MOST_NEAR = 6,
	ZTYPE_NUM,
};

enum TRANSTYPE
{
	TTYPE_DEFAULT = 0,
	TTYPE_NORMAL = 1,
	TTYPE_ADD = 2,
	TTYPE_DEC = 3,
	TTYPE_REVERSE = 4,
	TTYPE_NUM,
};

enum CTYPE
{
	CTYPE_DAMAGE = 0,
	CTYPE_ATTACK = 1,
	CTYPE_EXPOINT = 2,
	CTYPE_EXRECT = 3,
	CTYPE_EXVECTOR = 4,
	CTYPE_PUSH = 5,
	CTYPE_TEMP_CENTER = 6,
	CTYPE_NECK = 7,
	CTYPE_ABDOMINAL = 8,
	CTYPE_ATTACK_VS_PUSH = 9,
	CTYPE_SP_GUARD = 10,
	CTYPE_R_LEG = 11,
	CTYPE_L_LEG = 12,
	CTYPE_PRIVATE_POINT_0 = 13,
	CTYPE_PRIVATE_POINT_1 = 14,
	CTYPE_PRIVATE_POINT_2 = 15,
	CTYPE_PRIVATE_POINT_3 = 16,
	//this isnt in the tag decomp its added in strive
	CTYPE_EXTEND_JON = 17, 
	CTYPE_NUM
};

enum LayerType
{
	Normal = 0,
	Eyes = 1,
	Mouth = 2,
	LayerType_NUM
};

// SIZE: 0x50
class JonSpriteRect
{
public:
	Rectangle uvRect;		//0x00
	Rectangle positionRect;	//0x10
	ZTYPE zType;			//0x20
	float zOffset;			//0x24
	float alpha;			//0x28
	TRANSTYPE transType;	//0x2c
	float rotZ;				//0x30
	float unknown0;			//0x34
	float unknown1;			//0x38
	uint32_t spriteIndex;	//0x3c
	LayerType layerType;	//0x40
	uint32_t unknown3;		//0x44
	uint32_t palette;		//0x48
	uint32_t unknown4;		//0x4c
};

class JonCollisionRect
{
public:
	CTYPE collisionType;	//0x00
	Rectangle collisionRect;//0x04
};

class Jon
{
public:
	std::string jonName = "";

	std::vector<std::string> usedTextures = std::vector<std::string>();
	std::vector<JonSpriteRect> sprites = std::vector<JonSpriteRect>();
	std::vector<JonCollisionRect> collisions = std::vector<JonCollisionRect>();

	bool LoadFromFile(uint8_t* dataPtr, int fileSize);
};