#pragma once
#include "../error.h"

//implement cpps rule of three for all classes that would have vital data but not use reference counting system

//vertex buffer wrapper of opngl
class vertexBuffer
{
public:
	vertexBuffer() = default;
	vertexBuffer(vertexBuffer& va)
	{
		this->m_renderId = va.m_renderId;
		this->m_size = va.m_size;
		va.m_renderId = 0;
		va.m_size = 0;
	}
	//constructs array buffer from arguments
	vertexBuffer(void* data, unsigned int size, GLenum type = GL_DYNAMIC_DRAW);
	~vertexBuffer();

	void operator=(vertexBuffer&& va)
	{
		this->m_renderId = va.m_renderId;
		this->m_size = va.m_size;
		va.m_renderId = 0;
		va.m_size = 0;
	}

	//reads data ito pointer argument
	void getData(GLenum target, unsigned int offset, unsigned int size, void* data);
	// this is used for binding uniform buffers
	void bindBufferBase(GLenum target, unsigned int index);
	//clears data in te buffer
	void clearBuffer();
	//updates buffer data
	void updateBuffer(float* data, size_t size, int offset);

	//gets ID
	unsigned int getRendererID() { return m_renderId; }

	//sets buffer as the current buffer used for drawing vertexes by opengl
	void Bind();
	//unibinds
	void unBind();

private:
	//integer name for buffer ; it is recognized by opengl
	unsigned int m_renderId;
	unsigned m_size;
};


class IndexBuffer
{
public:
	IndexBuffer() = default;
	//constructs element array buffer from arguments
	IndexBuffer(unsigned int* data, unsigned int size, GLenum type = GL_DYNAMIC_DRAW);
	IndexBuffer(IndexBuffer& copy) //copy constructor
	{
		this->m_renderId = copy.m_renderId;
		copy.m_renderId = 0;
	}

	~IndexBuffer();

	void operator=(IndexBuffer&& va)
	{
		this->m_renderId = va.m_renderId;
		this->m_size = va.m_size;
		va.m_renderId = 0;
	}

	//updates index buffer
	void updateIndicies(unsigned int* data, int offset);

	//sets buffer as current buffer used by opengl for index data
	void Bind();
	//unbinds
	void unBind();

private:
	unsigned int m_renderId;
	unsigned m_size;
};


