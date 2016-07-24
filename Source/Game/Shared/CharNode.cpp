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

#ifdef _CLIENT
void CharNode::draw(Drawing& draw)
{
	this->draw(draw, glm::mat4(1.0f));
}

void CharNode::draw(Drawing& draw, glm::mat4 transform)
{
	transform *= glm::translate(m_pos);
	
	draw.setModel(transform);
	draw.coloredCube({ 0.0f, 0.0f }, 0.5f, Color::Blue(), 0.0f);

	transform *= m_rot;
	// draw cubes

	for(auto& n : m_nodes)
	{
		n->draw(draw, transform);
	}
}
#endif // _CLIENT
