#pragma once
#include "Shader.h"
#include "../../../Utility/Point.h"
#include "../../../glm/detail/type_vec3.hpp"

class ShaderButtonSide : public Shader
{
public:
	virtual ~ShaderButtonSide() override;
	virtual void load() override;
	virtual void create() override;
	void setStep(const PointF& s);
	void setLightPos(const glm::vec3& pos);
protected:
	virtual void loadUniforms() override;
private:
	GLint m_texLoc = -1;
	GLint m_heightLoc = -1;
	GLint m_step = -1;
	GLint m_light = -1;
};
