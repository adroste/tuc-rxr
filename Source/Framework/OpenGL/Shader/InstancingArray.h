#pragma once
#include "../GLObject.h"
#include <vector>
#include <cassert>

// vecType = glm::vec3 -> count = 3, enumType = GL_FLOAT
template <class vecType, size_t count, size_t enumType>
class InstacingArray : public GLObject
{
public:
	void create() override;
	void dispose() override;
	void bind(int slot);
	void setData(std::vector<vecType> data);
	void flush();
private:
	bool m_changed = true;
	GLuint m_vbo = 0;
	std::vector<vecType> m_data;
};

template <class vecType, size_t count, size_t enumType>
void InstacingArray<vecType, count, enumType >::create()
{
	assert(m_vbo == 0);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vecType) * m_data.size(),
		&m_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <class vecType, size_t count, size_t enumType>
void InstacingArray<vecType, count, enumType>::dispose()
{
	assert(m_vbo);
	glDeleteBuffers(1, &m_vbo);
	m_vbo = 0;
}

template <class vecType, size_t count, size_t enumType>
void InstacingArray<vecType, count, enumType>::bind(int slot)
{
	assert(m_vbo);
	glEnableVertexAttribArray(slot);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(slot, count, enumType,
		GL_FALSE, sizeof(vecType), nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(slot, 1);
}

template <class vecType, size_t count, size_t enumType>
void InstacingArray<vecType, count, enumType>::setData(std::vector<vecType> data)
{
	m_changed = true;
	m_data = move(data);
}

template <class vecType, size_t count, size_t enumType>
void InstacingArray<vecType, count, enumType>::flush()
{
	if (m_changed)
	{
		m_changed = false;
		// send to gpu
		if (m_vbo)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vecType) * m_data.size(),
				&m_data[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		else create();
	}
}
