#pragma once
#ifdef GRAPHICS_EXPORTS
#define GRAPHICS_API __declspec(dllexport) 
#else
#define GRAPHICS_API __declspec(dllimport)
#endif
#include "Graphics/GLHeaders.h"
#include <string>
#include <vector>
#include "TextFont.h"


struct OutputTextBlock
{
	uint32_t x;
	uint32_t y;
	std::string fontFilePath;
};

class TextBlock
{
public:
	GRAPHICS_API TextBlock(std::string *text, int fontSize, glm::vec4 *color, float x, float y, TextFont *font);
	float x;
	float y;
	int fontSize;
	std::string text;
	std::string fontFilePath;
	std::vector<OutputTextBlock*> outputs;
	TextFont *font;
	glm::vec4 color;
};

