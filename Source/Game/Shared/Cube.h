#pragma once
#include "CubeDesc.h"
#include "../../Framework/OpenGL/IDrawable.h"
#include "../../glm/detail/type_vec3.hpp"
#include "../../glm/gtx/transform.hpp"


class Cube : public IDrawable
{
public:
	Cube(const CubeDesc& desc, const glm::vec3& pos, bool isMapCube,float scalar = 1.0f);
	virtual ~Cube();
	void setPos(const glm::vec3& pos);
	void setRot(const glm::vec3& rot);
	const glm::vec3& getPos() const;
	const glm::vec3& getRot() const;

	const glm::mat4& getTransform() const;

	virtual void update(float dt){}

#ifdef _CLIENT
	virtual void draw(class Drawing& draw) override;
	//void draw(class Drawing& draw, glm::mat4 t);
#endif // _CLIENT

private:
	void recalcMatrix();

protected:
	const bool m_isMapCube;
private:
	CubeDesc m_desc;
	Color m_diffuse;
	Color m_specular;

	glm::vec3 m_pos;
	glm::vec3 m_rot;
	float m_scalar;
	glm::mat4 m_matTrans;
};
