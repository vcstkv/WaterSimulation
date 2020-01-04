#include "Graphics/Textures/Texture.h"



Texture::Texture(std::vector<uint8_t>&& image, std::string filePath, uint32_t id, uint32_t width, uint32_t height)
	:
	image(image),
	filePath(std::move(filePath)),
	id(id),
	width(width),
	height(height)
{
}

Texture::~Texture()
{
}

bool Texture::IsEqualFilePath(const std::string &filePath)
{
	return !this->filePath.compare(filePath);
}

uint32_t Texture::GetId()
{
	return id;
}

uint32_t Texture::GetWidth()
{
	return width;
}

uint32_t Texture::GetHeight()
{
	return height;
}


