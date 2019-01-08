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
	ShaderProgram();
	void Enable();
	void Disable();
	bool AddShader(GLenum type, const char* fileName);
	bool Link();
	~ShaderProgram();
protected:
	GLuint id;
private:
	GLuint LoadShader(GLenum type, const char* fileName);
	std::map<GLuint, const char*> *attachedShaders;
};

