#include "ShaderHSVPickerSquare.h"

void ShaderHSVPickerSquare::load()
{
	Shader::load("data/shader/hsvPickerSquare");
}

void ShaderHSVPickerSquare::loadUniforms()
{
	m_color = locateUniform("color");
}

void ShaderHSVPickerSquare::setColor(const Color& c)
{
	glUseProgram(m_program);
	glUniform4f(m_color, c.r, c.g, c.b, c.a);
	glUseProgram(0);
	glCheck("ShaderHSVPickerSquare::setColor");
}