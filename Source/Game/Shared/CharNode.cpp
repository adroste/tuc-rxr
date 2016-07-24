#include "CharNode.h"


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

void CharNode::rotate(const glm::mat4& rotation)
{
	m_rot *= rotation;
}

void CharNode::draw(Drawing& draw, glm::mat4 transform)
{
	transform *= glm::translate(m_pos);
	transform *= m_rot;

	// draw cubes
	//draw.setModel(transform);
	//draw.coloredCube({ 0.0f, 0.0f }, 0.5f, Color::Blue(), 0.0f);
	for (auto& cube : m_cubes)
	{
		cube->draw(draw, transform);
	}

	for(auto& n : m_nodes)
	{
		n->draw(draw, transform);
	}
}
#endif // _CLIENT
