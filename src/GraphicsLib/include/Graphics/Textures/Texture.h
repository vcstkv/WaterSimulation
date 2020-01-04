#pragma once

#include <string>

#include "PNGLoader.h"
#include "TGALoader.h"

class Texture
{
public:
	~Texture();
	Texture(std::vector<uint8_t>&& image, std::string filePath, uint32_t id, uint32_t width, uint32_t height);
	bool IsEqualFilePath(const std::string &filePath);
	uint32_t GetId();
	uint32_t GetWidth();
	uint32_t GetHeight();
private:
	std::vector<uint8_t> image;
	uint32_t id;
	uint32_t width;
	uint32_t height;
	std::string filePath;
};

