#pragma once
#include "Graphics/GLHeaders.h"
#include "Graphics/Shaders/SpriteShaderProgram.h"

class Line
{

public:
	GRAPHICS_API Line();
	GRAPHICS_API Line(float x1, float y1, float x2, float y2, float width, glm::vec4 *color);
	GRAPHICS_API ~Line();
	GRAPHICS_API void Draw(glm::mat4 *projection);
	GRAPHICS_API void SetLine(float x1, float y1, float x2, float y2, float width, glm::vec4 *color);

private:
	float width;
	void Init();
	static const int vertexCount = 2;
	glm::vec4 color;
	GLuint vertexBuffer;
	GLuint vertexArrayObject;
	SpriteShaderProgram* shaderProgram;
};

