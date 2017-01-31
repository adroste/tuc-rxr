#pragma once
#include "Shader.h"
#include "../../../Utility/Point.h"
#include <glm/glm.hpp>

class ShaderButton : public Shader, public Shader::ILoadable
{
public:
	virtual ~ShaderButton() override;
	virtual void load() override;
	virtual void create() override;
	void setLightPos(const glm::vec3& pos);
protected:
	virtual void loadUniforms() override;
private:
	GLint m_texLoc = -1;
	GLint m_heightLoc = -1;
	GLint m_light = -1;
};
