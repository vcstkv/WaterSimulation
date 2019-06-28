#pragma once
#include "Graphics/GLHeaders.h"
#include "Graphics/Shaders/ShaderProgram.h"

class CircleShaderProgram : public ShaderProgram
{
public:
	CircleShaderProgram(const char *vFilePath, const char *fFilePath);
	GLint mvpShLoc;
	GLint colorShLoc;
	GLint innerRadiusShLoc;
};

