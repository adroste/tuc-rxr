#pragma once
#include "Shader.h"
#include "../../Color.h"

class ShaderDisk : public Shader
{
public:
	virtual void load() override;
	void setColor(const Color& c);

protected:
	virtual void loadUniforms() override;

private:
	GLint m_color = -1;
};