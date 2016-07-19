#include "ShaderCube.h"

void ShaderCube::load()
{
	Shader::load("data/Shader/cube");
}

void ShaderCube::setColor(const Color& c)
{
	glUseProgram(m_program);
	glUniform4f(m_color, c.r, c.g, c.b, c.a);
	glUseProgram(0);
	glCheck("ShaderCube::setColor");
}

void ShaderCube::loadUniforms()
{
	m_color = glGetUniformLocation(m_program, "color");
	if (m_color == -1)
		throw Exception("ShaderCube::loadUniforms color uniform not found");
}
