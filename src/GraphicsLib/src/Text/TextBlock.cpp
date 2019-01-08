#define GRAPHICS_EXPORTS
#include "Graphics/Text/TextBlock.h"


TextBlock::TextBlock(std::string *text, int fontSize, glm::vec4 *color, float x, float y, TextFont *font)
{
	this->x = x;
	this->y = y;
	this->fontSize = fontSize;
	this->text = *text;
	this->font = font;
	this->color = *color;
}
