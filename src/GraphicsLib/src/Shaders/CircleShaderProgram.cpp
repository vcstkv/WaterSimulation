#define GRAPHICS_EXPORTS
#include "Graphics/Shaders/CircleShaderProgram.h"



CircleShaderProgram::CircleShaderProgram(const char *vFilePath, const char *fFilePath)
	: ShaderProgram()
{
	AddShader(GL_VERTEX_SHADER, vFilePath);
	AddShader(GL_FRAGMENT_SHADER, fFilePath);
	Link();

	mvpShLoc = glGetUniformLocation(id, "mvp");
	colorShLoc = glGetUniformLocation(id, "color");
	innerRadiusShLoc = glGetUniformLocation(id, "innerRadius");
}
