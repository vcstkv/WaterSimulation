#pragma once
#include "Sprite.h"
#include "Graphics/Shaders/SpriteShaderProgram.h"

class CommonSprite :
	public Sprite
{
public:
	GRAPHICS_API CommonSprite(float x, float y);
	GRAPHICS_API void Draw(glm::mat4 *projection, glm::mat4 *view);
	GRAPHICS_API void SetShader(const char *vFilePath, const char *fFilePath);

private:
	static std::vector<SpriteShaderProgram*> shaderPrograms;
};

