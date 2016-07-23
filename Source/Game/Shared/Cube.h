#pragma once
#include "CubeDesc.h"
#include "../../glm/detail/type_vec3.hpp"
#include "../../glm/gtx/transform.hpp"

#ifdef _CLIENT
#include "../Framework/OpenGL/Drawing.h"
#endif

class Cube
{
public:
	Cube(const CubeDesc& desc, const glm::vec3& pos, float scalar = 1.0f);
	virtual ~Cube();
	void setPos(const glm::vec3& pos);
	void setRot(const glm::vec3& rot);
	const glm::vec3& getPos() const;
	const glm::vec3& getRot() const;

	const glm::mat4& getTransform() const;

	virtual void update(float dt){}

#ifdef _CLIENT
	virtual void draw(class Drawing& draw);
#endif // _CLIENT
private:
	void recalcMatrix();

private:
	CubeDesc m_desc;

	glm::vec3 m_pos;
	glm::vec3 m_rot;
	float m_scalar;
	glm::mat4 m_matTrans;
};
