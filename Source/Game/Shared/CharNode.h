#pragma once
#include "../../glm/detail/type_vec3.hpp"
#include "../../glm/gtx/transform.hpp"
#include <vector>
#include <memory>
#include "../../Framework/OpenGL/IDrawable.h"
#include "Cube.h"

class CharNode : public IDrawable
{
	friend class Character;
public:
	CharNode(const glm::vec3& pos);
	virtual ~CharNode();

	void addNode(std::unique_ptr<CharNode> pNode);
	void setCubes(std::vector<std::unique_ptr<Cube>> cubes);

#ifdef _CLIENT
	void draw(Drawing& draw) override;
#endif // _CLIENT

	void rotate(float phi, float theta);

private:
#ifdef _CLIENT
	void draw(Drawing& draw, glm::mat4 transform);
#endif // _CLIENT

private:
	glm::vec3 m_pos;
	glm::mat4 m_rot;
	float m_phi = 0.0f;
	float m_theta = 0.0f;

	std::vector<std::unique_ptr<CharNode>> m_nodes;
	std::vector<std::unique_ptr<Cube>> m_cubes;
};
