#define GRAPHICS_EXPORTS
#include "Graphics/Text/TextFont.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>


void TextFont::DrawText(std::wstring text, float size, const glm::vec4 &color, float x, float y, const glm::mat4 &projection)
{
	scale.x = size / fontSize;
	scale.y = -scale.x;
	scale.z = 1.f;
	sprite->Scale(scale);
	sprite->SetPosition(x, y, 0);
	for (uint32_t i = 0, j = 0; i < text.size(); i++)
	{
		if (text[i] == L'\n')
		{
			sprite->Translate(glm::vec3(0, -fontSize, 0));
			continue;
		}
		j = 1 + (text[i] > 1000 ? (text[i] - 945 ) : (text[i] - 32));
		sprite->Translate(glyphs[j].width * 0.5f * scale.x, -glyphs[j].height * 0.5f * scale.x, 0);
		sprite->Translate(glyphs[j].xOffset * scale.x, -glyphs[j].yOffset* scale.x, 0);

		sprite->SetTextureShape(glyphs[j].x, glyphs[j].y, glyphs[j].width, glyphs[j].height);
		sprite->SetColor(color);
		sprite->Draw(projection, glm::mat4(1));

		sprite->Translate(-glyphs[j].xOffset* scale.x, glyphs[j].yOffset* scale.x, 0);
		sprite->Translate(-glyphs[j].width * 0.5f * scale.x, glyphs[j].height * 0.5f * scale.x, 0);
		sprite->Translate(glyphs[j].xAdvance * scale.x, 0, 0);
	}
	scale.x = 1.f / scale.x;
	scale.y = 1.f / scale.y;
	sprite->Scale(scale);
}


TextFont::TextFont(const char* filePath)
{
	LoadFont(filePath);
}


TextFont::~TextFont()
{
	free(glyphs);
	delete sprite;
}

void TextFont::ShowInfo()
{
	printf("\nTexture size:%fx%f", sprite->width, sprite->height);
	for (unsigned int i = 0; i < glyphsCount; i++)
	{
		printf("\nGlyph %d id:%d, x:%d, y:%d, w:%d, h:%d", i, glyphs[i].id, glyphs[i].x, glyphs[i].y, glyphs[i].width, glyphs[i].height);
	}
}

void TextFont::AddParamValue(const glm::vec4 &delta)
{
	sprite->AddParams(delta);
}

void TextFont::SetParamValue(const glm::vec4 &params)
{
	sprite->SetParams(params);
}

void TextFont::LoadFont(const char *filePath)
{
	uint32_t headerSize = 5;
	uint8_t  *buffer;
	uint32_t size;
	this->filePath = filePath;

	if (!FilesIOLibrary::LoadFile(filePath, &buffer, &size))
	{
		std::cout << "\nLoading font error";
		system("pause");
		exit(0);
		return;
	}

	uint32_t infoBlockLength = *(uint32_t*)&buffer[headerSize];
	uint32_t commonBlockLength = *(uint32_t*)&buffer[headerSize + 5 + infoBlockLength];
	uint32_t pagesBlockLength = *(uint32_t*)&buffer[headerSize + 5 + infoBlockLength + 5 + commonBlockLength];
	uint32_t charsBlockLength = *(uint32_t*)&buffer[headerSize + 5 + infoBlockLength + 5 + commonBlockLength + 5 + pagesBlockLength];

	fontSize = abs(*(int16_t*)&buffer[headerSize + 4])/8;

	lineHeight = (*(uint16_t*)&buffer[headerSize + 5 + infoBlockLength + 4])/8;

	base = (*(uint16_t*)&buffer[headerSize + 5 + infoBlockLength + 4 + 2])/8;

	char *textureFileName = (char*)malloc(sizeof(char) * pagesBlockLength);
	name = (char*)malloc(sizeof(char) * infoBlockLength - 14);

	for (uint32_t i = 0; i < infoBlockLength - 13; i++)
	{
		name[i] = *(uint8_t*)&buffer[headerSize + 5 + 13 + i];
	}

	for (uint32_t i = 0; i < pagesBlockLength; i++)
	{
		textureFileName[i] = *(uint8_t*)&buffer[headerSize + 5 + infoBlockLength + 5 + commonBlockLength + 4 + i];
	}

	using namespace std;

	string folderPath;
	bool isFolderPathStart = false;
	for(int i = strlen(filePath) - 1; i >= 0; i--)
	{
		if (!isFolderPathStart)
		{
			if (filePath[i] == '\\')
				isFolderPathStart = true;
		}
		else
		{
			folderPath = filePath[i] + folderPath;
		}
	}
	
	folderPath = folderPath + '\\' + textureFileName;

	sprite = new SDFChar(glm::vec2(0.0, 50));
	sprite->SetShader("SDFText.vs", "SDFText.fs");
	sprite->SetTexture(folderPath.c_str(), false);
	sprite->SetBorder(glm::vec4(1, 1, 0, 1), glm::vec2(0.5, 50));

	glyphsCount = charsBlockLength / 20;

	glyphs = (Glyph*)malloc(sizeof(Glyph) * glyphsCount);
	for (uint32_t i = 0; i < glyphsCount; i++)
	{
		glyphs[i] = *(Glyph*)&buffer[headerSize + 5 + infoBlockLength + 5 + commonBlockLength + 5 + pagesBlockLength + 4 + i * 20];
		glyphs[i].height /= 8;
		glyphs[i].width /= 8;
		glyphs[i].x /= 8;
		glyphs[i].y /= 8;
		glyphs[i].xOffset /= 8;
		glyphs[i].yOffset /= 8;
		glyphs[i].xAdvance /= 8;
	}

	free(textureFileName);
	delete[] buffer;
}

std::string TextFont::GetFilePath()
{
	return filePath;
}
