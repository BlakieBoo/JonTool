#include "Jon.hpp"

bool Jon::LoadFromFile(uint8_t* dataPtr, int fileSize)
{
	uint8_t* data = dataPtr;

	if (memcmp(data, "JONB", 4)) //not a jonbin
		return false;

	data += 4;
	
	uint16_t texCount = *(uint16_t*)data;
	data += 2;

	//keep track of expected size, if wrong most likely a granblue jon, which have padding between boxes
	int expectedSize = 7 + (32 * texCount);

	usedTextures.clear();
	usedTextures.resize(texCount);
	for (int i = 0; i < texCount; i++)
	{
		usedTextures[i] = *(C32BYTE*)data;
		usedTextures[i] = usedTextures[i].substr(0, usedTextures[i].find_last_of("."));
		usedTextures[i].resize(32);
		data += sizeof(C32BYTE);
	}

	uint8_t elementCount = *data;
	if (elementCount > CTYPE_NUM + 3) //atm CTYPE_NUM Is the max amount of boxes arcsys uses, but in case more is added we wont load 
		return false;

	data += 1;
	expectedSize += elementCount * 2;

	uint16_t* elements = (uint16_t*)data;

	uint16_t padCount1 = elements[0];
	expectedSize += 52 * padCount1;
	uint16_t spriteCount = elements[1];
	expectedSize += spriteCount * sizeof(JonSpriteRect);
	uint16_t padCount2 = elements[2];
	expectedSize += 308 * padCount2;

	uint8_t collisionCount = elementCount - 3;

	uint16_t collisionBoxCounts[CTYPE_NUM] = { 0 };
	for (int i = 0; i < CTYPE_NUM; i++)
		collisionBoxCounts[i] = 0;

	for (int i = 0; i < collisionCount; i++)
	{
		collisionBoxCounts[i] = elements[i + 3];
		expectedSize += collisionBoxCounts[i] * sizeof(JonCollisionRect);
	}

	//file size smaller for some reason, this is not good
	if (expectedSize < fileSize)
		return false;

	bool gbvs = expectedSize > fileSize;

	data += 2 * elementCount + 52 * padCount1;

	sprites.clear();
	sprites.resize(spriteCount);

	for (int i = 0; i < spriteCount; i++)
	{
		sprites[i] = *(JonSpriteRect*)data;
		data += sizeof(JonSpriteRect);
	}

	//have never seen a jonbin use this but bbtag skips these so im just gonna do that here too
	data += 308 * padCount2;

	collisions.clear();
	int boxCount = 0;
	for (int i = 0; i < CTYPE_NUM; i++)
		boxCount += collisionBoxCounts[i];
	collisions.resize(boxCount);

	int curIndex = 0;
	for (int i = 0; i < CTYPE_NUM; i++)
		for (int i2 = 0; i2 < collisionBoxCounts[i]; i2++)
		{
			collisions[curIndex] = *(JonCollisionRect*)data;
			data += sizeof(JonCollisionRect);
			if (gbvs)
				data += 4;
			curIndex++;
		}

	return true;
}