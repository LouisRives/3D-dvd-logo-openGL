#pragma once
#include "buffers.h"
#include <vector>
#include <string>

enum class DataType : short int
{
	FLOAT = 1,
	INT = 2,
	BYTE = 3,
	UNSIGNED_INT = 4,
	UNSIGNED_BYTE = 5
};

//struct container for data used in setting vertexattribute layout
struct bufferElements
{
	bufferElements() = default;
	bufferElements(int Count, GLenum DataType, bool isNormalized, const void* Beginning);
	~bufferElements() = default;

	int elementCount;
	GLenum type;
	bool normalized;
	const void* BeginIter;
};


class BufferLayout
{
public:
	BufferLayout() = default;

	//pushes a layout into m_elements
	void push(bufferElements elements);
	//uses argument to construct buffer element  thats pushed into m_elements
	void push(unsigned int count, DataType type, bool normalized);
	//uses argument to construct buffer element  for non interleaved attributes thats pushed into m_elements
	void push(unsigned int count, DataType type, bool normalized, const void* iter);
	//gets all elements
	const std::vector<bufferElements>& get() { return m_elements; }

	void flush() {
		stride = 0;
		currSlot = 0;
		offsetCount = 0;
		m_elements.erase(m_elements.begin(), m_elements.end());
	}

	unsigned int stride = 0;
private:
	//stores all element	//ellements arw layout for how opengl uses the vague data given to
	//m_elements could contain layouts for vertices, colors, texture coordinates, normals etc
	std::vector<bufferElements> m_elements;
	int currSlot = 0;
	int offsetCount = 0;

};

class vertexArray
{
public:
	vertexArray() = default;
	//vertexArray(const vertexArray&& va)noexcept;//causes double allocation of memory when just one is needed
	vertexArray(bool t);
	~vertexArray();

	void operator=(vertexArray&& va)
	{
		this->m_renderId = va.m_renderId;
		va.m_renderId = 0;
	}

	void Bind();
	void unBind();

	//sets the layout which opengl uses to identify data stored in  m_data
	void addBuffer(BufferLayout& layout, vertexBuffer& m_data);
	void addBuffer(BufferLayout& layout, vertexBuffer& m_data, int start, int iter = 0);
private:
	unsigned int m_renderId;
};
