#pragma once
#include "../GLObject.h"
#include <vector>

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