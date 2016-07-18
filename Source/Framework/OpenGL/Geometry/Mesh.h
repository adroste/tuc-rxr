#pragma once
#include "../GLObject.h"
#include "../../../glm/glm.hpp"
#include <vector>

class Mesh : public GLObject
{
	enum class AttributeName
	{
		Position,
		Normal,
		Index,
		
		Size
	};
public:
	struct Vertex
	{
		Vertex(const glm::vec3& pos, const glm::vec3& normal)
			:
			pos(pos),normal(normal)
		{}
		Vertex()
		{}
		glm::vec3 pos;
		glm::vec3 normal;
	};
public:
	Mesh(const Vertex* pVertex, size_t nVertices, const unsigned int* pIndices, size_t nIndices);
	virtual ~Mesh();

	void draw();

	void create() override;
	void dispose() override;

private:
	static const unsigned int NUM_BUFFERS = unsigned int(AttributeName::Size);

	std::vector<decltype(Vertex::pos)> m_positions;
	std::vector<decltype(Vertex::normal)> m_normal;
	std::vector<unsigned int> m_indices;

	GLuint m_vertexArrayObject = 0;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
};