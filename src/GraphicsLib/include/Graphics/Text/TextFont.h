#pragma once
#ifdef GRAPHICS_EXPORTS
#define GRAPHICS_API __declspec(dllexport) 
#else
#define GRAPHICS_API __declspec(dllimport)
#endif
#include "Graphics/GLHeaders.h"
#include "Graphics/Shaders/SdfTextShaderProgram.h"
#include "Graphics/Shapes/Sprites/SDFChar.h"
#include <string>
#include "FilesIO/FilesIOLibrary.h"


struct Glyph
{
	uint32_t id;
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	int16_t xOffset;
	int16_t yOffset;
	int16_t xAdvance;
};

class TextFont
{
public:
	GRAPHICS_API void DrawText(std::wstring text, int size, glm::vec4 *color, float x, float y, glm::mat4 *projection);
	GRAPHICS_API TextFont(char* filePath);
	GRAPHICS_API ~TextFont();
	GRAPHICS_API void AddParamValue(glm::vec4 *delta);
	GRAPHICS_API void SetParamValue(glm::vec4 *params);
	GRAPHICS_API char* GetFilePath();
	GRAPHICS_API void ShowInfo();
private:
	void LoadFont(char *filePath);
	Glyph *glyphs;
	SDFChar *sprite;
	uint32_t glyphsCount;
	int16_t fontSize;
	uint16_t lineHeight;
	uint16_t base;
	glm::vec3 scale;
	char* name;
	char* filePath;
	
};

