#pragma once
#include "../../glm/detail/type_vec3.hpp"
#include "../../glm/gtx/transform.hpp"
#include <vector>
#include <memory>
#include "../../Framework/OpenGL/IDrawable.h"

class CharNode : public IDrawable
{
public:
	CharNode(const glm::vec3& pos);
	virtual ~CharNode();

	void addNode(std::unique_ptr<CharNode> pNode);

#ifdef _CLIENT
	void draw(Drawing& draw) override;
#endif // _CLIENT

private:
#ifdef _CLIENT
	void draw(Drawing& draw, glm::mat4 transform);
#endif // _CLIENT

private:
	glm::vec3 m_pos;
	glm::mat4 m_rot;

	std::vector<std::unique_ptr<CharNode>> m_nodes;
	// TODO cubes[]
};
