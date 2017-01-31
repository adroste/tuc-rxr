#include "CharNode.h"
#include <glm/gtc/matrix_transform.hpp>


CharNode::CharNode(const glm::vec3& pos)
	:
	m_pos(pos),
	m_rot(1.0f)
{
}

CharNode::~CharNode()
{
}

void CharNode::addNode(std::unique_ptr<CharNode> pNode)
{
	m_nodes.push_back(std::move(pNode));
}

void CharNode::setCubes(std::vector<std::unique_ptr<Cube>> cubes)
{
	m_cubes = std::move(cubes);
}

#ifdef _CLIENT
void CharNode::draw(Drawing& draw)
{
	this->draw(draw, glm::mat4(1.0f));
}

void CharNode::rotate(float phi, float theta)
{
	m_phi += phi;
	m_theta += theta;
	glm::mat4 m = glm::rotate(glm::mat4(), m_phi, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec4 z =  m * glm::vec4(0.0f, 0.0f, 1.0f,0.0f);
	m = glm::rotate(glm::mat4(), m_theta, glm::vec3(z.x,z.y,z.z)) * m;
	m_rot = m;
}

void CharNode::draw(Drawing& draw, glm::mat4 transform)
{
	transform *= glm::translate(glm::mat4(), m_pos);
	transform *= m_rot;

	// draw cubes
	//draw.setModel(transform);
	//draw.coloredCube({ 0.0f, 0.0f }, 0.5f, Color::Blue(), 0.0f);
	draw.getTransform().pushModel(transform);
	for (auto& cube : m_cubes)
	{
		cube->draw(draw);
	}

	for(auto& n : m_nodes)
	{
		n->draw(draw);
	}
	draw.getTransform().popModel();
}
#endif // _CLIENT
