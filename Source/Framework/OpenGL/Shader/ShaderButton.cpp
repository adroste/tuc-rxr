#include "ShaderButton.h"

ShaderButton::~ShaderButton()
{
}

void ShaderButton::load()
{
	Shader::load("data/Shader/button");
}

void ShaderButton::create()
{
	Shader::create();

	glUseProgram(m_program);
	glUniform1i(m_texLoc, 0);
	glUniform1i(m_heightLoc, 1);
	glUseProgram(0);

	glCheck("ShaderButton::create");
}

void ShaderButton::setLightPos(const glm::vec3& pos)
{
	glUseProgram(m_program);
	glUniform3f(m_light, pos.x, pos.y, pos.z);
	glUseProgram(0);
}

void ShaderButton::loadUniforms()
{
	m_texLoc = locateUniform("tex");
	m_heightLoc = locateUniform("heightMap");
	m_light = locateUniform("lightPos");
}
