#pragma once
#include "Shader.h"
#include "../../Color.h"

class ShaderHSVPickerSquare : public Shader, public Shader::ILoadable
{
public:
	virtual void load() override;
	void setColor(const Color& c);

protected:
	virtual void loadUniforms() override;

private:
	GLint m_color = -1;
};