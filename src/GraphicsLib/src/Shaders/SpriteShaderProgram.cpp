#include "Graphics/Shaders/SpriteShaderProgram.h"



SpriteShaderProgram::SpriteShaderProgram(const char *vFilePath, const char *fFilePath) 
	: ShaderProgram()
{
	AddShader(GL_VERTEX_SHADER, vFilePath);
	AddShader(GL_FRAGMENT_SHADER, fFilePath);
	Link();
	mvpShLoc = glGetUniformLocation(id, "MVP");
	matDiffColorShLoc = glGetUniformLocation(id, "materialDiffuseColor");
	texSamplerShLoc = glGetUniformLocation(id, "textureSampler");
	isColoredShLoc = glGetUniformLocation(id, "isColoredTexture");
}
