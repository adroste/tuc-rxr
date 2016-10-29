#include "ShaderCubeMap.h"

void ShaderCubeMap::load()
{
	Shader::load("data/Shader/cube_map");
}

void ShaderCubeMap::loadUniforms()
{
	m_chHeight = locateUniform("chunkHeight");
}
