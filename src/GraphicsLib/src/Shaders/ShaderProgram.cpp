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
}

GLuint ShaderProgram::LoadShader(GLenum type, const char *fileName)
{
	char path[128];
	strcpy(path, GraphicsResources::shadersFolderPath);
	strcat(path, fileName);

	printf("\nLoading shader: %s", fileName);

	uint8_t *src;
	uint32_t srcLength;
	if (!FilesIOLibrary::LoadFile(path, &src, &srcLength))
	{
		return 0;
	}

	printf("\nCompiling shader: %s", fileName);
	GLuint sid = glCreateShader(type);
	char const *source = (char const*)src;
	glShaderSource(sid, 1, &source, (GLint*)&srcLength);
	glCompileShader(sid);

	GLint result = GL_FALSE;
	glGetShaderiv(sid, GL_COMPILE_STATUS, &result);
	if (result == GL_TRUE)
	{
		printf(" OK!\n");
	}
	else
	{
		int InfoLogLength;
		glGetShaderiv(sid, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> errorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(sid, InfoLogLength, NULL, &errorMessage[0]);
		printf(" Error!\n%s\n", &errorMessage[0]);
		glDeleteShader(sid);
		sid = 0;
	}
	delete src;
	return sid;
}

bool ShaderProgram::AddShader(GLenum type, const char *fileName)
{
	printf("\nAdding Shader %s", fileName);
	if (id == 0)
	{
		printf(" Error!\nThe program is not created.");
		return false;
	}
	GLuint sid;
	if ((sid = LoadShader(type, fileName)) == 0)
	{
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
	if (result == GL_FALSE)
	{
		int InfoLogLength;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf(" Error!\n%s", &ProgramErrorMessage[0]);
		return false;
	}
	printf(" OK!\n");
	return true;
}