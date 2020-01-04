#pragma once

#include "Graphics/Shapes/Sprites/Sprite.h"
#include "Graphics/Shaders/CircleShaderProgram.h"
#include "Graphics/GLHeaders.h"

class Circle : public Sprite
{

public:
	GRAPHICS_API Circle();
	GRAPHICS_API Circle(const glm::vec3 &c, float radius, float width, const glm::vec4 &color);
	GRAPHICS_API ~Circle();
	GRAPHICS_API void Draw(const glm::mat4 &projection, const glm::mat4 &view);
	GRAPHICS_API void SetCircle(const glm::vec3 &c, float radius, float width, const glm::vec4 &color);
	GRAPHICS_API void SetShader(const char *vFilePath, const char *fFilePath);

private:
	void Init();
	float innerRadius = 0.f;
	CircleShaderProgram* shaderProgram = nullptr;
};

