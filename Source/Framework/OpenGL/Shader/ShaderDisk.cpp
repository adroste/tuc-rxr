#include "ShaderDisk.h"

void ShaderDisk::load()
{
	Shader::load("data/shader/disk");
}

void ShaderDisk::loadUniforms()
{
	m_color = locateUniform("color");
}

void ShaderDisk::setColor(const Color& c)
{
	glUseProgram(m_program);
	glUniform4f(m_color, c.r, c.g, c.b, c.a);
	glUseProgram(0);
	glCheck("ShaderDisk::setColor");
}