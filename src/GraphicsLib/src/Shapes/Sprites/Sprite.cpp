#define GRAPHICS_EXPORTS
#include "Graphics/Shapes/Sprites/Sprite.h"
#include <iostream>
#include <vector>

Sprite::Sprite(const glm::vec3 &c)
{
	InitializeSprite();

	SetPosition(c);
}

Sprite::~Sprite()
{
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &textureBuffer);
}

void Sprite::InitializeSprite()
{
	float vertices[] =
	{ 
		-0.5f, -0.5f, 0.f,
		-0.5f,  0.5f, 0.f,
		 0.5f,  0.5f, 0.f,
		 0.5f, -0.5f, 0.f
	};

	float uvs[] =
	{ 
		1.f, 1.f,
		1.f, 0.f,
		0.f, 0.f,
		0.f, 1.f
	};

	unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * dim, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, dim, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 2, uvs, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::SetSize(float width, float height)
{
	Scale(glm::vec3(width/this->width, height/this->height, 1));
	this->width = width;
	this->height = height;
}

void Sprite::SetTextureShape(float uvX, float uvY, int width, int height)
{
	SetSize(width, height);
	int tW = this->texturesController->GetTextureWidth(this->textureId);
	int tH = this->texturesController->GetTextureHeight(this->textureId);
	float uvs[] =
	{
		uvX / tW,                     uvY / tH,
		uvX / tW,                     uvY / tH + height * 1.f / tH,
		uvX / tW + width * 1.f / tW,  uvY / tH + height * 1.f / tH,
		uvX / tW + width * 1.f / tW,  uvY / tH
	};

	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertexCount * 2, uvs);
}

void Sprite::Scale(const glm::vec3 &delta)
{
	model = glm::scale(model, delta);
}

void Sprite::SetScale(const glm::vec3 &scale)
{
	model = glm::scale(scale);
}

void Sprite::SetPosition(float x, float y, float z)
{
	model[3].x = x;
	model[3].y = y;
	model[3].z = z;
}

void Sprite::SetPosition(const glm::vec3 &position)
{
	model[3].x = position.x;
	model[3].y = position.y;
	model[3].z = position.z;
}

void Sprite::Translate(float dx, float dy, float dz)
{
	model[3].x += dx;
	model[3].y += dy;
	model[3].z += dz;
}

void Sprite::Translate(const glm::vec3 &dPos)
{
	model[3].x += dPos.x;
	model[3].y += dPos.y;
	model[3].z += dPos.z;
}

void Sprite::SetTexture(const char* filePath, bool isStdFolder)
{
	textureId = texturesController->AddTexture(filePath, isStdFolder);
	SetSize(texturesController->GetTextureWidth(textureId), texturesController->GetTextureHeight(textureId));
}

void Sprite::SetActiveShaderProgram(int localShaderProgramId)
{
	if (localShaderProgramId < 0)
	{
		std::cout << "Shader program id < 0";
		return;
	}
	currentShaderProgram = localShaderProgramId;
}

void Sprite::SetColor(const glm::vec4 &color)
{
	this->color = color;
}

TexturesController *Sprite::texturesController;
void Sprite::SetTexturesController(TexturesController *_texturesController)
{
	texturesController = _texturesController;
}


