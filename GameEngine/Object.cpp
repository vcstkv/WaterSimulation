#include "Object.h"

Object::Object(float *vertices, float *indices, float *normals, glm::vec3 *position, GLuint shadingProgramID)
{
	this->shadingProgramID = shadingProgramID;
	vertices = (float*)malloc(sizeof(float) * VERTICES_COUNT * 2);

	model = glm::mat4(1);
	this->position = *position;

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * VERTICES_COUNT * 3, vertices, GL_STATIC_DRAW);

	mvpShLoc = glGetUniformLocation(shadingProgramID, "MVP");
	colorShLoc = glGetUniformLocation(shadingProgramID, "color");
}


Object::~Object()
{
	glDeleteBuffers(1, &vertexBuffer);
	delete vertices;
}

GLfloat color[3] = {1.0, 0.0, 1.0};

void Object::Draw(glm::mat4 *projection, glm::mat4 *view)
{
	glUseProgram(shadingProgramID);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glUniformMatrix4fv(mvpShLoc, 1, GL_FALSE, &((*projection) * (*view) * model)[0][0]);
	glUniform4fv(colorShLoc, 1, &color[0]);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES_COUNT);
	glDisableVertexAttribArray(0);
}




