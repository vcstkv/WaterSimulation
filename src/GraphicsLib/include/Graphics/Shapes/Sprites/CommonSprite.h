#pragma once
#include "Sprite.h"
#include "Graphics/Shaders/SpriteShaderProgram.h"

class CommonSprite :
	public Sprite
{
public:
	GRAPHICS_API CommonSprite(const glm::vec3 &p);
	GRAPHICS_API void Draw(const glm::mat4 &projection, const glm::mat4 &view);
	GRAPHICS_API void SetShader(const char *vFilePath, const char *fFilePath);
	GRAPHICS_API void PrintInfo();
private:
	static std::vector<SpriteShaderProgram*> shaderPrograms;
};

