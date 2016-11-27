#pragma once
#include "../GLObject.h"
#include "../../../glm/glm.hpp"
#include <vector>
#include "../Shader/InstancingArray.h"
#include "../../../System/Exceptions/GL_Exception.h"

class Mesh : public GLObject
{
	enum class AttributeName
	{
		Position,
		Normal,
		TexCoord0,
		Index,
		
		Size
	};
public:
	struct Vertex
	{
		Vertex(const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& texCoord0)
			:
			pos(pos),normal(normal), texCoord0(texCoord0)
		{}
		Vertex()
		{}
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texCoord0;
	};
public:
	Mesh(const Vertex* pVertex, size_t nVertices, const unsigned int* pIndices, size_t nIndices);
	virtual ~Mesh();

	void draw();

	template <class vecType, size_t count, size_t enumType>
	void drawInstanced(size_t num, InstancingArray<vecType, count, enumType>& ia);

	void create() override;
	void dispose() override;

private:
	static const unsigned int NUM_BUFFERS = static_cast<unsigned int>(AttributeName::Size);

	std::vector<decltype(Vertex::pos)> m_positions;
	std::vector<decltype(Vertex::normal)> m_normal;
	std::vector<decltype(Vertex::texCoord0)> m_texCoords0;
	std::vector<unsigned int> m_indices;

	GLuint m_vertexArrayObject = 0;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
};

template <class vecType, size_t count, size_t enumType>
void Mesh::drawInstanced(size_t num, InstancingArray<vecType, count, enumType>& ia)
{
	glBindVertexArray(m_vertexArrayObject);

	ia.bind(3);
	glCheck("l Mesh::drawInstaced");
	glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr, num);
	glCheck("Mesh::drawInstaced");

	glBindVertexArray(0);
}