#pragma once
#include "PNGLoader.h"
#include "TGALoader.h"

class Texture
{
public:
	~Texture();
	Texture(std::vector<uint8_t> *image, const char *filePath, uint32_t id, uint32_t width, uint32_t height);
	bool IsEqualFilePath(const char *filePath);
	int GetId();
	int GetWidth();
	int GetHeight();
private:
	std::vector<uint8_t> *image;
	uint32_t id;
	uint32_t width;
	uint32_t height;
	const char *filePath;
};

