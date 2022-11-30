#include "IndexBuffer.h"

Creek::Graphics::IndexBuffer::IndexBuffer(const unsigned int* size, unsigned int count) : count(count)
{
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), size, GL_STATIC_DRAW);
}
Creek::Graphics::IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &ibo);
}

void Creek::Graphics::IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void Creek::Graphics::IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
