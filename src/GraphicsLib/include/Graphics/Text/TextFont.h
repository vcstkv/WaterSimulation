#pragma once
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
	GRAPHICS_API void DrawText(std::wstring text, float size, const glm::vec4 &color, float x, float y, const glm::mat4 &projection);
	GRAPHICS_API TextFont(const char* filePath);
	GRAPHICS_API ~TextFont();
	GRAPHICS_API void AddParamValue(const glm::vec4 &delta);
	GRAPHICS_API void SetParamValue(const glm::vec4 &params);
	GRAPHICS_API std::string GetFilePath();
	GRAPHICS_API void ShowInfo();
private:
	void LoadFont(const char *filePath);
	Glyph *glyphs = nullptr;
	SDFChar *sprite = nullptr;
	uint32_t glyphsCount = 0;
	int16_t fontSize = 0;
	uint16_t lineHeight = 0;
	uint16_t base = 0;
	glm::vec3 scale{ 1.f };
	char* name = "";
	std::string filePath;
	
};

