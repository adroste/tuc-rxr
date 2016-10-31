#include "ShaderCubeMap.h"

void ShaderCubeMap::load()
{
	Shader::load("data/Shader/cube_map");
}

void ShaderCubeMap::setChunkHeight(size_t h)
{
	glUseProgram(m_program);
	glUniform1ui(m_chHeight, h);
	glUseProgram(0);
}

void ShaderCubeMap::loadUniforms()
{
	m_chHeight = locateUniform("chunkHeight");
}
