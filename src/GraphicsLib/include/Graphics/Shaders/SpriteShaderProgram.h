#pragma once
#include "Graphics/GLHeaders.h"
#include "Graphics/Shaders/ShaderProgram.h"

class SpriteShaderProgram : public ShaderProgram
{
public:
	SpriteShaderProgram(const char *vFilePath, const char *fFilePath);
	GLint mvpShLoc;
	GLint texSamplerShLoc;
	GLint matDiffColorShLoc;
	GLint isColoredShLoc;
	
};

