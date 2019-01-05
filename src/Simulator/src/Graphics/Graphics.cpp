#define GRAPHICS_EXPORTS
#include "Graphics/Graphics.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>

Graphics::Graphics()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 1.f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
}

using namespace std;
bool GetShaderCode(string* code, const char* filePath)
{
	ifstream fileStream;
	string line;
	fileStream.open(filePath, ios::in);
	if (fileStream)
	{
		while (getline(fileStream, line))
		{
			*code += "\n" + line;
		}
		fileStream.close();
		return true;
	}
	else
	{
		printf("Can not open %s.\n", filePath);
		getchar();
		return false;
	}
}

GLuint Graphics::LoadShaders(const char* vFilePath, const char* fFilePath)
{   
	GLuint vId = LoadShader(vFilePath, GL_VERTEX_SHADER);
	GLuint fId = LoadShader(fFilePath, GL_FRAGMENT_SHADER);

	if (vId == 0 || fId == 0)
	{
		return 0;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	printf("\nLinking program");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vId);
	glAttachShader(ProgramID, fId);
	glLinkProgram(ProgramID);
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	if (Result == GL_TRUE) printf(" OK!\n");
	else printf(" Error!\n%s\n", &ProgramErrorMessage[0]);


	glDeleteShader(vId);
	glDeleteShader(fId);
	return ProgramID;
}

GLuint Graphics::LoadShader(const char* path, GLenum type)
{
	GLuint id = glCreateShader(type);

	string code;

	if (!GetShaderCode(&code, path))
	{
		return 0;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	printf("\nCompiling shader: %s", path);
	char const * VertexSourcePointer = code.c_str();
	glShaderSource(id, 1, &VertexSourcePointer, NULL);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);

	std::vector<char> errorMessage(InfoLogLength + 1);
	glGetShaderInfoLog(id, InfoLogLength, NULL, &errorMessage[0]);
	if (Result == GL_TRUE) printf(" OK!\n");
	else printf(" Error!\n%s\n", &errorMessage[0]);

	return id;
}

GLuint Graphics::LoadBitmap(const char* filePath)
{
	return TextureCreateFromTGA(filePath);
}

	