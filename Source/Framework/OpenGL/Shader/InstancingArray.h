#pragma once
#include "../GLObject.h"
#include <vector>
#include <cassert>
#include "../../Framework.h"
#include "../../../Utility/Mutex.h"

// vecType = glm::vec3 -> count = 3, enumType = GL_FLOAT
template <class vecType, size_t count, size_t enumType>
class InstancingArray : public GLObject
{
public:
	void create() override;
	void dispose() override;
	void bind(int slot);
	void setData(std::vector<vecType> data);
	void flush();
	size_t getDataCount() const; 
private:
	bool m_changed = true;
	gl::ArrayBuffer m_vbo;
	std::vector<vecType> m_data;
	Mutex m_muData;
};

template <class vecType, size_t count, size_t enumType>
void InstancingArray<vecType, count, enumType >::create()
{
	m_vbo.create();
	m_vbo.bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vecType) * m_data.size(),
		&m_data[0], GL_STATIC_DRAW);
	m_vbo.unbind();
}

template <class vecType, size_t count, size_t enumType>
void InstancingArray<vecType, count, enumType>::dispose()
{
	m_vbo.dispose();
}

template <class vecType, size_t count, size_t enumType>
void InstancingArray<vecType, count, enumType>::bind(int slot)
{
	glEnableVertexAttribArray(slot);
	m_vbo.bind();
	glVertexAttribPointer(slot, count, enumType,
		GL_FALSE, 0, nullptr);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(slot, 1);
}

template <class vecType, size_t count, size_t enumType>
void InstancingArray<vecType, count, enumType>::setData(std::vector<vecType> data)
{
	LockGuard g(m_muData);
	m_changed = true;
	m_data = move(data);
}

template <class vecType, size_t count, size_t enumType>
void InstancingArray<vecType, count, enumType>::flush()
{
	DRAW_THREAD;
	if (m_changed)
	{
		LockGuard g(m_muData);
		m_changed = false;
		// send to gpu
		if (m_vbo.get())
		{
			m_vbo.bind();
			glBufferData(GL_ARRAY_BUFFER, sizeof(vecType) * m_data.size(),
				&m_data[0], GL_STATIC_DRAW);
			m_vbo.unbind();
		}
		else create();
	}
}

template <class vecType, size_t count, size_t enumType>
size_t InstancingArray<vecType, count, enumType>::getDataCount() const
{
	return m_data.size();
}
