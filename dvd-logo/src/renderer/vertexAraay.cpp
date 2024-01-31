#include "vertexArray.h"



size_t getSizeof(DataType data_type)
{
	switch (data_type)
	{
	case DataType::FLOAT:
		return sizeof(float);
		break;
	case DataType::BYTE:
		return sizeof(char);
		break;
	case DataType::UNSIGNED_INT:
		return sizeof(unsigned int);
	default:
		return 0;
		break;
	}
}

GLenum getGlType(DataType data_type)
{
	switch (data_type)
	{
	case DataType::FLOAT:
		return GL_FLOAT;
		break;
	case DataType::BYTE:
		return GL_BYTE;
		break;
	case DataType::UNSIGNED_BYTE:
		return GL_UNSIGNED_BYTE;
		break;
	case DataType::UNSIGNED_INT:
		return GL_UNSIGNED_INT;
	default:
		return GL_NONE;
		break;
	}
}


vertexArray::vertexArray(bool t)
{
	GLcall(glGenVertexArrays(1, &m_renderId));
	GLcall(glBindVertexArray(m_renderId));
}

vertexArray::~vertexArray()
{
	GLcall(glDeleteVertexArrays(1, &m_renderId));
}

void vertexArray::Bind()
{
	GLcall(glBindVertexArray(m_renderId));
}

void vertexArray::unBind()
{
	GLcall(glBindVertexArray(0));
}

void vertexArray::addBuffer(BufferLayout& layout, vertexBuffer& m_data)
{
	const std::vector<bufferElements>& elements = layout.get();
	Bind();
	m_data.Bind();
	for (size_t i = 0; i < elements.size(); i++)
	{
		GLcall(glVertexAttribPointer(i, elements[i].elementCount, elements[i].type, elements[i].normalized, layout.stride, elements[i].BeginIter));
		GLcall(glEnableVertexAttribArray(i));
	}
}

void vertexArray::addBuffer(BufferLayout& layout, vertexBuffer& m_data, int start, int iter)
{
	const std::vector<bufferElements>& elements = layout.get();
	Bind();
	m_data.Bind();

	int i = 0;

		GLcall(glVertexAttribPointer(start, elements[i].elementCount, elements[i].type, elements[i].normalized, layout.stride, (const void*)(iter) ));
		GLcall(glEnableVertexAttribArray(start));
	
}

bufferElements::bufferElements(int Count, GLenum DataType, bool isNormalized, const void* Beginning)
	:elementCount(Count), type(DataType), normalized(isNormalized), BeginIter(Beginning)
{
}

void BufferLayout::push(unsigned int  count, DataType type, bool normalized)
{
	offsetCount += count;
	stride += (count * getSizeof(type));
	m_elements.push_back(bufferElements());
	m_elements[currSlot].elementCount = count;
	m_elements[currSlot].BeginIter = (const void*)((offsetCount - count) * getSizeof(type));
	m_elements[currSlot].type = getGlType(type);
	m_elements[currSlot].normalized = normalized;
	currSlot++;
}

void BufferLayout::push(unsigned int count, DataType type, bool normalized, const void* iter)
{
	offsetCount += count;
	stride += 0;//for non interleaved buffers stride is 0
	m_elements.push_back(bufferElements());
	m_elements[currSlot].elementCount = count;
	m_elements[currSlot].BeginIter = iter;
	m_elements[currSlot].type = getGlType(type);
	m_elements[currSlot].normalized = normalized;
	currSlot++;
}

void BufferLayout::push(bufferElements elements)
{
	m_elements.push_back(elements);
}