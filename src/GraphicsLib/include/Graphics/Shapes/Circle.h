#pragma once

#include "Graphics/Shapes/Sprites/Sprite.h"
#include "Graphics/Shaders/CircleShaderProgram.h"
#include "Graphics/GLHeaders.h"

class Circle : public Sprite
{

public:
	GRAPHICS_API Circle();
	GRAPHICS_API Circle(float x, float y, float radius, float width, glm::vec4 *color);
	GRAPHICS_API ~Circle();
	GRAPHICS_API void Draw(glm::mat4 *projection, glm::mat4 *view);
	GRAPHICS_API void SetCircle(float x, float y, float radius, float width, glm::vec4 *color);
	GRAPHICS_API void SetShader(const char *vFilePath, const char *fFilePath);

private:
	void Init();
	float innerRadius;
	CircleShaderProgram* shaderProgram;
};

