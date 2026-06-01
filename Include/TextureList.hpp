#pragma once

#include "Common.hpp"

void LoadTex(std::string& file);
bool TexIsLoaded(std::string& tex);
void UnloadTex(std::string& tex);
void ClearTexList();
Texture2D* GetTexture(std::string& tex);
bool TexQueuedToLoad();
void UploadQueuedTextures();