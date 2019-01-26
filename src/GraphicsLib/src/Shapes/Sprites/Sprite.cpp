#define GRAPHICS_EXPORTS
#include "Graphics/Shapes/Sprites/Sprite.h"
#include <iostream>
#include <vector>

Sprite::Sprite(float x, float y)
{
	color = glm::vec4(-1);
	textureId = 0;
	
	InitializeSprite();

	model = glm::scale(glm::vec3(-1, 1, 1));
	SetPosition(x, y);
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
	float vertices[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	float uvs[] = { 1, 1, 1, 0, 0, 0, 0, 1 };

	unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 2, vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

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

void Sprite::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;
	float vertices[] =
	{
		-width / 2, -height / 2,
		-width / 2,  height / 2,
		 width / 2,  height / 2,
		 width / 2, -height / 2
	};

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertexCount * 2, vertices);
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

void Sprite::Scale(glm::vec3 *delta)
{
	model = glm::scale(model, *delta);
}

void Sprite::SetScale(glm::vec3 *scale)
{
	model = glm::scale(*scale);
}

void Sprite::SetPosition(float x, float y)
{
	model[3].x = x;
	model[3].y = y;
}

void Sprite::SetPosition(glm::vec3 *position)
{
	model[3].x = position->x;
	model[3].y = position->y;
}

void Sprite::Translate(float dx, float dy)
{
	model[3].x += dx;
	model[3].y += dy;
}

void Sprite::Translate(glm::vec3 *dPos)
{
	model[3].x += dPos->x;
	model[3].y += dPos->y;
}

void Sprite::SetTexture(const char* filePath, bool isStdFolder)
{
	textureId = texturesController->AddTexture(filePath, isStdFolder);
	width = texturesController->GetTextureWidth(textureId);
	height = texturesController->GetTextureHeight(textureId);
	SetSize(width, height);
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

void Sprite::SetColor(glm::vec4 *color)
{
	this->color = *color;
}

TexturesController *Sprite::texturesController;
void Sprite::SetTexturesController(TexturesController *_texturesController)
{
	texturesController = _texturesController;
}


