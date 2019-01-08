#pragma once
#include "Graphics/Shapes/Sprites/Sprite.h"
#include "Graphics/Shaders/SdfTextShaderProgram.h"

class SDFChar :
	public Sprite
{
public:
	GRAPHICS_API SDFChar(glm::vec2 *drawParams);
	GRAPHICS_API void Draw(glm::mat4 *projection, glm::mat4 *view);
	GRAPHICS_API void SetShader(const char *vFilePath, const char *fFilePath);
	GRAPHICS_API void SetBorder(glm::vec4 *borderColor, glm::vec2 *borderDrawParams);
	GRAPHICS_API void DeleteBorder();
	GRAPHICS_API void SetBorderParams(glm::vec2 *params);
	GRAPHICS_API void SetCharParams(glm::vec2 *params);
	GRAPHICS_API void SetParams(glm::vec4 *params);
	GRAPHICS_API void AddParams(glm::vec4 *delta);


private:
	glm::vec4 borderColor;
	glm::vec4 params;
	static std::vector<SdfTextShaderProgram*> shaderPrograms;
};

