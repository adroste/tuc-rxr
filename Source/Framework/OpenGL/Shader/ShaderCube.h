#pragma once
#include "Shader.h"
#include "../../Color.h"

class ShaderCube : public Shader
{
public:
	virtual ~ShaderCube() {}
	void load() override;

	void setColor(const Color& c);
	
protected:
	virtual void loadUniforms() override;

private:
	GLint m_color = -1;
};
