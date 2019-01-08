#pragma once
#include <vector>
#include "TextFont.h"
#include <string>

class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();
	void DrawText(std::string text, float x, float y, int size);
	void DrawText(std::string text, float x, float y, int size, int rectColor);
	void DrawText(std::string text, float x, float y, int size, int outlineColor, int outlineWidth);
	void SetFont(char* font);
private:
	std::vector<TextFont*> *fontsList;
};

