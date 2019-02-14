#define GRAPHICS_EXPORTS
#include "Graphics/Shapes/Line.h"
#include <iostream>


Line::Line()
{
	Init();
}

Line::Line(float x1, float y1, float x2, float y2, float width, glm::vec4 *color)
{
	Init();
	SetLine(x1, y1, x2, y2, width, color);
}

void Line::Init()
{
	float init[] = { 0, 0, 0, 0 };

	width = 1.;

	glGenBuffers(1, &vertexBuffer);
	glGenVertexArrays(1, &vertexArrayObject);

	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 2, init, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	shaderProgram = new SpriteShaderProgram("CommonSprite.vs", "CommonSprite.fs");
}



Line::~Line()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexArrayObject);
	delete shaderProgram;
}

void Line::Draw(glm::mat4 *projection)
{
	shaderProgram->Enable();
	
	glUniformMatrix4fv(shaderProgram->mvpShLoc, 1, GL_FALSE,  &(*projection)[0][0]);
	glUniform4fv(shaderProgram->matDiffColorShLoc, 1, &(color)[0]);
	glUniform1i(shaderProgram->isColoredShLoc, 0);
	glBindVertexArray(vertexArrayObject);
	glEnableVertexAttribArray(0);
	glLineWidth(width);
	glDrawArrays(GL_LINES, 0, vertexCount * 2);
	glLineWidth(1.);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	shaderProgram->Disable();
}

void Line::SetLine(float x1, float y1, float x2, float y2, float width, glm::vec4 *color)
{
	this->color = *color;
	this->width = width;

	float vertices[] = { x1, y1, x2, y2 };

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertexCount * 2, vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
