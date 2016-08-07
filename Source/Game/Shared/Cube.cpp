#include "Cube.h"
#include "../../Framework/OpenGL/Shader/Shader.h"
#include "../../Framework/OpenGL/Shader/ShaderCube.h"

Cube::Cube(const CubeDesc & desc, const glm::vec3 & pos, float scalar)
	:
	m_desc(desc),
	m_diffuse(desc.diffuse),
	m_specular(desc.spec),
	m_pos(pos),
	m_rot(0.0f,0.0f,0.0f),
	m_scalar(scalar * 0.5f + 0.001f)
{
	recalcMatrix();
}

Cube::~Cube()
{
}

void Cube::setPos(const glm::vec3& pos)
{
	m_pos = pos;
	recalcMatrix();
}

void Cube::setRot(const glm::vec3& rot)
{
	m_rot = rot;
	recalcMatrix();
}

const glm::vec3& Cube::getPos() const
{
	return m_pos;
}

const glm::vec3& Cube::getRot() const
{
	return  m_rot;
}

const glm::mat4& Cube::getTransform() const
{
	return m_matTrans;
}

#ifdef _CLIENT
void Cube::draw(Drawing& draw)
{
	ShaderCube& shader = draw.getShaderCubeMap();
	draw.setCubeMaterial(m_diffuse, m_specular, m_desc.gloss);

	draw.shaderedCube(m_matTrans, shader);
}

//void Cube::draw(Drawing& draw, glm::mat4 t)
//{
//	ShaderCube& shader = draw.getShaderCubeMap();
//	draw.setCubeMaterial(m_diffuse, m_specular, m_desc.gloss);
//
//	draw.shaderedCube(t * m_matTrans, shader);
//}
#endif // _CLIENT

void Cube::recalcMatrix()
{
	m_matTrans = glm::translate(m_pos);
	m_matTrans *= glm::scale(glm::vec3(m_scalar));
	if (m_rot.x != 0.0f)
		m_matTrans *= glm::rotate(m_rot.x, glm::vec3(1.0f, 0.0f, 0.0f));

	if (m_rot.y != 0.0f)
		m_matTrans *= glm::rotate(m_rot.y, glm::vec3(0.0f, 1.0f, 0.0f));

	if (m_rot.z != 0.0f)
		m_matTrans *= glm::rotate(m_rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
}
