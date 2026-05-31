#include "TextureList.hpp"

#include <unordered_map>
#include <algorithm>

static std::unordered_map<std::string, Texture2D> textureList = std::unordered_map<std::string, Texture2D>();

void LoadTex(std::string& file)
{
	std::string texName = file;
	std::replace(texName.begin(), texName.end(), '\\', '/');
	texName = texName.substr(texName.find_last_of('/') + 1);
	texName = texName.substr(0, texName.find_last_of('.'));

	if (textureList.contains(texName))
		UnloadTex(texName);

	Texture2D tex = LoadTexture(file.c_str());
	SetTextureWrap(tex, TEXTURE_WRAP_CLAMP);
	textureList.insert({ texName, tex });
	std::cout << texName << std::endl;
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