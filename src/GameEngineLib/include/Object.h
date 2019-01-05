#pragma once
#include"Graphics.h"
#include <vector>

#define PI 3.14159265
//#include "vector.h"

#define VERTICES_COUNT 6

class Object
{
private:
	GLfloat *vertices;
protected:
	Object(float *vertices, float *indices, float *normals, glm::vec3 *position, GLuint shadingProgramID);
	~Object();
	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint normalBuffer;
	GLuint shadingProgramID;
	GLuint mvpShLoc;
	GLuint colorShLoc;
	void Draw(glm::mat4 *projection, glm::mat4 *view);
	glm::vec3 position;
	glm::mat4 model;
};

