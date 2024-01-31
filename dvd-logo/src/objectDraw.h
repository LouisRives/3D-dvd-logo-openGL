#pragma once
#include <iostream>
#include <string>
#include "error.h"
#include "Animation.h"


class Object
{
public:
	
	Object(float* vertex_data, unsigned int vsize, unsigned int* index_data = nullptr, unsigned int index_size = 0)
	{
		GLcall(glGenVertexArrays(1, &VAO));
		GLcall(glBindVertexArray(VAO));

		GLcall(glGenBuffers(1, &m_buff));
		GLcall(glBindBuffer(GL_ARRAY_BUFFER, m_buff));
		GLcall(glBufferData(GL_ARRAY_BUFFER, vsize, vertex_data, GL_STATIC_DRAW));

		GLcall(glEnableVertexAttribArray(0));
		GLcall(glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (const void*)(0) ));

		GLcall(glEnableVertexAttribArray(1));
		GLcall(glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (const void*)(3 * sizeof(float) )));

		if (index_data)
		{
			index_data_ = true;
			GLcall(glGenBuffers(1, &m_elements));
			GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elements));
			GLcall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, index_data, GL_STATIC_DRAW));
		}
	}


	void Draw_object(unsigned int count)
	{
		GLcall(glBindVertexArray(VAO));
		GLcall(glBindBuffer(GL_ARRAY_BUFFER, m_buff));

		if (index_data_)
		{
			GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elements));
			GLcall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
			return;
		}
		else
		{
			GLcall(glad_glDrawArrays(GL_TRIANGLES, 0, 36));
		}

		//GLcall(glDrawArrays(GL_TRIANGLE_STRIP, 0, count));
	}

private:
	bool index_data_ = 0;
	unsigned int  m_buff;
	unsigned int  m_elements;
	unsigned int VAO;
};
