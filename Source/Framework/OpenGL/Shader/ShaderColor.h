#pragma once
#include "Shader.h"
#include "../../Color.h"

class ShaderColor : public Shader
{
public:
	ShaderColor();
	~ShaderColor() override;
	virtual void load() override;
	void setColor(const Color& c);
protected:
	
	virtual void loadUniforms() override;

private:
	GLint m_colorLoc = -1;
};