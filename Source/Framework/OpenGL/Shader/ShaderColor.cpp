#include "ShaderColor.h"

ShaderColor::ShaderColor()
{
}

ShaderColor::~ShaderColor()
{
}

void ShaderColor::load()
{
	Shader::load("data/Shader/color");
}

void ShaderColor::setColor(const Color& c)
{
	glUseProgram(m_program);
	glUniform4f(m_colorLoc, c.r, c.g, c.b, c.a);
	glUseProgram(0);
}

void ShaderColor::loadUniforms()
{
	m_colorLoc = locateUniform("color");
}
