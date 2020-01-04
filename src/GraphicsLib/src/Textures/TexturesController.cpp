#define GRAPHICS_EXPORTS
#include "Graphics/Textures/TexturesController.h"
#include "Graphics/GraphicsResources.h"


TexturesController::TexturesController()
{
	AddTexture("errorTexture.png");
}


TexturesController::~TexturesController()
{
}


uint32_t TexturesController::AddTexture(std::string filePath, bool isStdFolder)
{
	for (uint32_t i = 0; i < textures.size(); i++)
	{
		if (textures[i]->IsEqualFilePath(filePath))
		{
			return i + 1;
		}
	}
		
	std::string ext;
	size_t dotPos = filePath.rfind('.');
	if (dotPos == std::string::npos)
	{
		return 1;
	}
	ext = filePath.substr(dotPos + 1);

	if (isStdFolder)
	{
		filePath.insert(0, GraphicsResources::texturesFolderPath);
	}

	std::vector<uint8_t> image;
	uint32_t width;
	uint32_t height;
	uint32_t error;
	int format;
	int internalFormat;
	if (!ext.compare("png"))
	{
		error = lodepng::decode(image, width, height, filePath);
		if (error != 0)
		{
			std::cout << "\nload error " << error << ": " << lodepng_error_text(error) << ": \"" << filePath.data() << "\"";
			return 1;
		}
		format = GL_RGBA;
		internalFormat = GL_RGBA;
	}
	else if (!ext.compare("tga"))
	{
		error = LoadTGA(image, width, height, internalFormat, format, filePath);
		if (error != 0)
		{
			return 1;
		}
	}
	else
	{
		printf("\nLoad error: \"%s\". Unsupported format", filePath.data());
		return 1;
	}


	uint32_t textureId;

	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, &image[0]);

	std::cout << "\nTexture loaded: \"" << filePath.data() << "\"";
	textures.emplace_back(std::make_unique<Texture>(std::move(image), std::move(filePath), textureId, width, height));

	return textureId;
}

int TexturesController::GetTextureWidth(uint32_t textureId)
{
	return textureId - 1 < textures.size() ? textures[textureId - 1]->GetWidth() : 0;
}

int TexturesController::GetTextureHeight(uint32_t textureId)
{
	return textureId - 1 < textures.size() ? textures[textureId - 1]->GetHeight() : 0;
}


