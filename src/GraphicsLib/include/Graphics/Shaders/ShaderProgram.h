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
	GRAPHICS_API GLuint CreateCompositeShader(GLenum type);
	GRAPHICS_API void AddPartialShaderFile(GLuint compositeShader, const char* fileName);
	GRAPHICS_API void CompileCompositeShader(GLuint compositeShader);
	GRAPHICS_API virtual ~ShaderProgram();
protected:
	GLuint id;
private:
	bool LoadShader(GLuint sid, const char *fileName);
	std::string LoadShaderCode(GLuint sid, const char *fileName);
	bool CompileShader(GLuint sid, const std::string &src);
	std::map<GLuint, std::string> compositeShadersCodeBuffer;
	std::map<GLuint, const char*> attachedShaders;
};

