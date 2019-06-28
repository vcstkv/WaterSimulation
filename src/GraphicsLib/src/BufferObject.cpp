#include "BufferObject.h"
#include <vector>

BufferObject::BufferObject(GLenum type, GLuint binding, GLuint size)
{
	glGenBuffers(1, &id);
	std::vector<unsigned int> v(size);

	// now bind this new buffer to the dedicated buffer binding location
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, id);

	// and fill it with 0s
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
	glBufferData(GL_SHADER_STORAGE_BUFFER, v.size() * sizeof(unsigned int), v.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

BufferObject::~BufferObject()
{
	glDeleteBuffers(1, &id);
}