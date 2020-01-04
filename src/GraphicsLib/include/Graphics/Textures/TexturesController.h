#pragma once
#include "Graphics/GLHeaders.h"
#include "Graphics/Textures/Texture.h"
#include <iostream>
#include <vector>
#include <string>


class TexturesController
{
public:
	TexturesController();
	~TexturesController();
	uint32_t AddTexture(std::string filePath, bool isStdFolder = true);
	int GetTextureWidth(uint32_t textureId);
	int GetTextureHeight(uint32_t textureId);
private:
	std::vector<std::unique_ptr<Texture>> textures;
};

