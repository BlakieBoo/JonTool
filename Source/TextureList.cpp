#include "TextureList.hpp"

#include <unordered_map>
#include <algorithm>
#include <mutex>
#include <queue>

static std::unordered_map<std::string, Texture2D> textureList = std::unordered_map<std::string, Texture2D>();

static std::mutex queueMutex = std::mutex();
static std::queue<Image> imageQueue = std::queue<Image>();
static std::queue<std::string> queueNames = std::queue<std::string>();

void LoadTex(std::string& file)
{

	std::string texName = file;
	std::replace(texName.begin(), texName.end(), '\\', '/');
	texName = texName.substr(texName.find_last_of('/') + 1);
	texName = texName.substr(0, texName.find_last_of('.'));

	Image img = LoadImage(file.c_str());
	std::lock_guard lock = std::lock_guard(queueMutex);
	imageQueue.push(img);
	queueNames.push(texName);
}

bool TexIsLoaded(std::string& tex)
{
	return textureList.contains(tex);
}

void UnloadTex(std::string& tex)
{
	UnloadTexture(textureList[tex]);
	textureList.erase(tex);
}

void ClearTexList()
{
	for (auto kvp : textureList)
		UnloadTexture(kvp.second);

	textureList.clear();
}

Texture2D* GetTexture(std::string& tex)
{
	if (textureList.contains(tex))
		return &textureList[tex];

	return nullptr;
}

bool TexQueuedToLoad()
{
	std::lock_guard lock = std::lock_guard(queueMutex);
	return imageQueue.size();
}

void UploadQueuedTextures()
{
	std::lock_guard lock = std::lock_guard(queueMutex);
	for (int i = 0; i < 100 && imageQueue.size(); i++)
	{
		std::string texName = queueNames.front();
		if (textureList.contains(texName))
			UnloadTex(texName);
		Texture2D tex = LoadTextureFromImage(imageQueue.front());
		SetTextureWrap(tex, TEXTURE_WRAP_CLAMP);
		textureList.insert({ texName, tex});
		UnloadImage(imageQueue.front());
		imageQueue.pop();
		queueNames.pop();
	}
}