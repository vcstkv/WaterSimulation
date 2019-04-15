#define GRAPHICS_EXPORTS
#include "Graphics/Shaders/ShaderProgram.h"
#include "Graphics/GraphicsResources.h"

ShaderProgram::ShaderProgram()
{
	printf("\nCreating shader program");
	id = glCreateProgram();
	if (id > 0)
	{
		printf(" OK, id: %d", id);
		attachedShaders = new std::map<GLuint, const char*>();
		compositeShadersCodeBuffer = new std::map<GLuint, std::string>();
	}
	else
	{
		printf(" Error");
	}
}

void ShaderProgram::Enable()
{
	if (id > 0)
	{
		glUseProgram(id);
	}
}

void ShaderProgram::Disable()
{
	glUseProgram(0);
}

ShaderProgram::~ShaderProgram()
{
	if (id > 0)
	{
		glDeleteProgram(id);
		attachedShaders->clear();
		delete attachedShaders;
	}
	if (compositeShadersCodeBuffer)
	{
		compositeShadersCodeBuffer->clear();
		delete compositeShadersCodeBuffer;
	}
}

std::string ShaderProgram::LoadShaderCode(GLuint sid, const char *fileName)
{
	std::string src;
	char path[128];
	strcpy(path, GraphicsResources::shadersFolderPath);
	strcat(path, fileName);

	printf("\nLoading shader %d: %s", sid, fileName);

	uint8_t *buffer;
	uint32_t srcLength;
	if (!FilesIOLibrary::LoadFile(path, &buffer, &srcLength))
	{
		return src;
	}
	src.append((char*)buffer, srcLength);
	delete[] buffer;

	return src;
}

bool ShaderProgram::CompileShader(GLuint sid, std::string &src)
{
	printf("\nCompiling shader: %d", sid);

	GLint srcLen = src.size();

	const GLchar * srcData = src.data();
	glShaderSource(sid, 1, &srcData, (GLint*)&srcLen);
	glCompileShader(sid);

	GLint result = GL_FALSE;
	glGetShaderiv(sid, GL_COMPILE_STATUS, &result);
	if (result == GL_TRUE)
	{
		printf(" OK!\n");
		return true;
	}

	int InfoLogLength;
	glGetShaderiv(sid, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> errorMessage(InfoLogLength + 1);
	glGetShaderInfoLog(sid, InfoLogLength, NULL, &errorMessage[0]);
	printf(" Error!\n%s\n", &errorMessage[0]);
	return false;
}

bool ShaderProgram::LoadShader(GLuint sid, const char *fileName)
{
	std::string src = LoadShaderCode(sid, fileName);
	return !src.empty() && CompileShader(sid, src);
}

bool ShaderProgram::AddShader(GLenum type, const char *fileName)
{
	printf("\nAdding Shader %s", fileName);
	if (id == 0)
	{
		printf(" Error!\nThe program is not created.");
		return false;
	}
	GLuint sid = glCreateShader(type);
	if (sid == 0 || !LoadShader(sid, fileName))
	{
		glDeleteShader(sid);
		return false;
	}
	glAttachShader(id, sid);
	return true;
}

bool ShaderProgram::Link()
{
	printf("\nLinking shader program %d", id);
	if (id == 0)
	{
		printf(" Error!\nThe program is not created.");
		return false;
	}

	glLinkProgram(id);

	GLint result = GL_FALSE;
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (result == GL_TRUE)
	{
		printf(" OK!\n");
		return true;	
	}
	int InfoLogLength;
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
	glGetProgramInfoLog(id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	printf(" Error!\n%s", &ProgramErrorMessage[0]);
	return false;
}

GLint ShaderProgram::GetUniformLocation(const GLchar *name)
{
	if (id == 0)
	{
		return 0;
	}
	return glGetUniformLocation(id, name);
}

GLuint ShaderProgram::CreateCompositeShader(GLenum type)
{
	GLuint sid;
	if (!compositeShadersCodeBuffer || ((sid = glCreateShader(type)) == 0))
	{
		return 0;
	}

	compositeShadersCodeBuffer->insert(std::pair<GLuint, std::string>(sid, std::string()));
	return sid;
}

void ShaderProgram::AddPartialShaderFile(GLuint compositeShader, const char* fileName)
{
	if (!compositeShadersCodeBuffer)
	{
		return;
	}
	auto it = compositeShadersCodeBuffer->find(compositeShader);
	if (it != compositeShadersCodeBuffer->end())
	{
		it->second.append("\n" + LoadShaderCode(compositeShader, fileName));
	}
}

void ShaderProgram::CompileCompositeShader(GLuint compositeShader)
{
	if (!compositeShadersCodeBuffer)
	{
		return;
	}
	auto it = compositeShadersCodeBuffer->find(compositeShader);
	if (it != compositeShadersCodeBuffer->end())
	{
		CompileShader(compositeShader, it->second);
	}
	glAttachShader(id, compositeShader);
	compositeShadersCodeBuffer->erase(it);
}
