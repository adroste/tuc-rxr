#include "ShaderDisk.h"

void ShaderDisk::load()
{
	Shader::load("data/shader/disk");
}

void ShaderDisk::loadUniforms()
{
	m_color = glGetUniformLocation(m_program, "color");
	if (m_color == -1)
		throw Exception("disk shader: color uniform not found");
}

void ShaderDisk::setColor(const Color& c)
{
	glUseProgram(m_program);
	glUniform4f(m_color, c.r, c.g, c.b, c.a);
	glUseProgram(0);
	glCheck("ShaderDisk::setColor");
}