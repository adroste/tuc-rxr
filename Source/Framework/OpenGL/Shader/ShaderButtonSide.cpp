#include "ShaderButtonSide.h"

ShaderButtonSide::~ShaderButtonSide()
{
}

void ShaderButtonSide::load()
{
	Shader::load("data/Shader/btnSide");
}

void ShaderButtonSide::create()
{
	Shader::create();

	glUseProgram(m_program);
	glUniform1i(m_texLoc, 0);
	glUniform1i(m_heightLoc, 1);
	glUseProgram(0);

	glCheck("ShaderButtonSide::create");
}

void ShaderButtonSide::setStep(const PointF& s)
{
	glUseProgram(m_program);
	glUniform2f(m_step, s.x, s.y);
	glUseProgram(0);
}

void ShaderButtonSide::setLightPos(const glm::vec3& pos)
{
	glUseProgram(m_program);
	glUniform3f(m_light, pos.x, pos.y, pos.z);
	glUseProgram(0);
}

void ShaderButtonSide::loadUniforms()
{
	m_texLoc = glGetUniformLocation(m_program, "tex");
	if (m_texLoc == -1)
		throw Exception("ShaderButtonSide::loadUniforms tex uniform not found");

	m_heightLoc = glGetUniformLocation(m_program, "heightMap");
	if (m_heightLoc == -1)
		throw Exception("ShaderButtonSide::loadUniforms heightMap uniform not found");

	m_step = glGetUniformLocation(m_program, "d");
	if (m_step == -1)
		throw Exception("ShaderButtonSide::loadUniforms d uniform not found");

	m_light = glGetUniformLocation(m_program, "lightPos");
	if (m_light == -1)
		throw Exception("ShaderButtonSide::loadUniforms lightPos uniform not found");
}
