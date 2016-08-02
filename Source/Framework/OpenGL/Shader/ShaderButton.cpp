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
	m_texLoc = glGetUniformLocation(m_program, "tex");
	if (m_texLoc == -1)
		throw Exception("ShaderButton::loadUniforms tex uniform not found");

	m_heightLoc = glGetUniformLocation(m_program, "heightMap");
	if (m_heightLoc == -1)
		throw Exception("ShaderButton::loadUniforms heightMap uniform not found");

	m_light = glGetUniformLocation(m_program, "lightPos");
	if (m_light == -1)
		throw Exception("ShaderButton::loadUniforms lightPos uniform not found");
}
