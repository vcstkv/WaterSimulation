#pragma once
#include "Graphics/Shapes/Sprites/Sprite.h"
#include "Graphics/Shaders/SdfTextShaderProgram.h"

class SDFChar :
	public Sprite
{
public:
	GRAPHICS_API SDFChar(const glm::vec2 &drawParams);
	GRAPHICS_API void Draw(const glm::mat4 &projection, const glm::mat4 &view);
	GRAPHICS_API void SetShader(const char *vFilePath, const char *fFilePath);
	GRAPHICS_API void SetBorder(const glm::vec4 &borderColor, const glm::vec2 &borderDrawParams);
	GRAPHICS_API void DeleteBorder();
	GRAPHICS_API void SetBorderParams(const glm::vec2 &params);
	GRAPHICS_API void SetCharParams(const glm::vec2 &params);
	GRAPHICS_API void SetParams(const glm::vec4 &params);
	GRAPHICS_API void AddParams(const glm::vec4 &delta);


private:
	glm::vec4 borderColor;
	glm::vec4 params;
	static std::vector<SdfTextShaderProgram*> shaderPrograms;
};

