#pragma once
#include "Graphics/GLHeaders.h"
#include "Graphics/Shaders/SpriteShaderProgram.h"

class Line
{

public:
	GRAPHICS_API Line();
	GRAPHICS_API Line(const glm::vec3 &b, const glm::vec3 &e, float width, const glm::vec4 &color);
	GRAPHICS_API ~Line();
	GRAPHICS_API void Draw(const glm::mat4 &projection, const glm::mat4 &view);
	GRAPHICS_API void SetLine(const glm::vec3 &b, const glm::vec3 &e, float width, const glm::vec4 &color);

private:
	void Init();
	float width = 1.f;
	static const int vertexCount = 2;
	static const int dim = 3;
	glm::vec4 color = glm::vec4(0.);
	GLuint vertexBuffer = 0;
	GLuint vertexArrayObject = 0;
	SpriteShaderProgram* shaderProgram = nullptr;
};

