#include "Mesh.h"
#include "../../../System/Exceptions/GL_Exception.h"

Mesh::Mesh(const Vertex * pVertex, size_t nVertices, const unsigned int * pIndices, size_t nIndices)
{
	memset(m_vertexArrayBuffers, 0, sizeof(m_vertexArrayBuffers));
	for(size_t i = 0; i < nVertices; i++)
	{
		m_positions.push_back(pVertex[i].pos);
		m_normal.push_back(pVertex[i].normal);
		m_texCoords0.push_back(pVertex[i].texCoord0);
	}
	for (size_t i = 0; i < nIndices; i++)
		m_indices.push_back(pIndices[i]);
}

Mesh::~Mesh()
{
}

void Mesh::draw()
{
	glBindVertexArray(m_vertexArrayObject);

	//glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glDrawElementsBaseVertex(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr, 0);
	glCheck("Mesh::draw");

	glBindVertexArray(0);
}




void Mesh::create()
{
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);

	// init buffers (positions, normals etc)
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[static_cast<unsigned int>(AttributeName::Position)]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex::pos) * m_positions.size(), &m_positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(GLuint(AttributeName::Position));
	glVertexAttribPointer(GLuint(AttributeName::Position), 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[static_cast<unsigned int>(AttributeName::Normal)]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex::normal) * m_normal.size(), &m_normal[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(GLuint(AttributeName::Normal));
	glVertexAttribPointer(GLuint(AttributeName::Normal), 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[static_cast<unsigned int>(AttributeName::TexCoord0)]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex::texCoord0) * m_texCoords0.size(), &m_texCoords0[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(GLuint(AttributeName::TexCoord0));
	glVertexAttribPointer(GLuint(AttributeName::TexCoord0), 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[static_cast<unsigned int>(AttributeName::Index)]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices[0]) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glCheck("Mesh::create");
}

void Mesh::dispose()
{
	glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glDeleteVertexArrays(1, &m_vertexArrayObject);

	m_vertexArrayObject = 0;
	memset(m_vertexArrayBuffers, 0, sizeof(m_vertexArrayBuffers));
}
