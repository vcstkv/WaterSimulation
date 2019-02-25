#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "Graphics/GLHeaders.h"
#include "FilesIO/FilesIOLibrary.h"


class ShaderProgram
{
public:
	GRAPHICS_API ShaderProgram();
	GRAPHICS_API void Enable();
	GRAPHICS_API void Disable();
	GRAPHICS_API bool AddShader(GLenum type, const char* fileName);
	GRAPHICS_API bool Link();
	GRAPHICS_API GLint GetUniformLocation(const GLchar* name);
	GRAPHICS_API ~ShaderProgram();
protected:
	GLuint id;
private:
	GLuint LoadShader(GLenum type, const char* fileName);
	std::map<GLuint, const char*> *attachedShaders;
};

