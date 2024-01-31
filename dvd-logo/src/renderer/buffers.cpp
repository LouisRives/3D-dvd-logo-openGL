#include "buffers.h"


vertexBuffer::vertexBuffer(void* data, unsigned int size, GLenum type)
	:m_size(size)
{
	GLcall(glGenBuffers(1, &m_renderId));
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, m_renderId));
	GLcall(glBufferData(GL_ARRAY_BUFFER, size, data, type));
}

vertexBuffer::~vertexBuffer()
{
	GLcall(glDeleteBuffers(1, &m_renderId));
}

void vertexBuffer::getData(GLenum target, unsigned int offset, unsigned int size, void* data)
{
	GLcall(glBindBuffer(target, m_renderId));
	GLcall(glGetBufferSubData(target, offset, size, data));
	return;
}

void vertexBuffer::bindBufferBase(GLenum target, unsigned int index)
{
	GLcall(glBindBufferBase(target, index, m_renderId));
}

void vertexBuffer::clearBuffer()
{
	Bind();
	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	delete ptr;
	ptr = nullptr;
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void vertexBuffer::updateBuffer(float* data, size_t size, int offset)
{
	//M_ASSERT((sizeof(data) <= m_size));
	Bind();
	int val = 1;
	GLcall(glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &val));
	GLcall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}


void vertexBuffer::Bind()
{
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, m_renderId));
}

void vertexBuffer::unBind()
{
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

/*vertexArray::vertexArray(const vertexArray&& va)
{
	this->m_renderId = va.m_renderId;
}*/

IndexBuffer::IndexBuffer(unsigned int* data, unsigned int size, GLenum type)
{
	m_size = size;
	GLcall(glGenBuffers(1, &m_renderId));
	GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderId));
	GLcall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, type));
}

IndexBuffer::~IndexBuffer()
{
	if(m_renderId != 0)
	{
		GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderId));
		GLcall(glDeleteBuffers(1, &m_renderId));
	}
	else
	{

	}
}


void IndexBuffer::updateIndicies(unsigned int* data, int offset)
{
	Bind();
	GLcall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, sizeof(data), data));
}

void IndexBuffer::Bind()
{
	GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderId));
}

void IndexBuffer::unBind()
{
	GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
