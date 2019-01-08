#pragma once
#include "Graphics/GLHeaders.h"
#include "Graphics/Shaders/ShaderProgram.h"

class SdfTextShaderProgram : public ShaderProgram
{
public:
	SdfTextShaderProgram(const char *vFilePath, const char *fFilePath);
	GLint mvpShLoc;
	GLint texSamplerShLoc;
	GLint paramsShLoc;
	GLint colorShLoc;
	GLint borderColorShLoc;
};

