#include "ShaderCubeMap.h"
#include "../../../Game/Shared/MapLoader.h"

void ShaderCubeMap::load()
{
	Shader::load("data/Shader/cube_map");
}

void ShaderCubeMap::setAnimation(AssetAnimation a)
{
	bind();
	glUniform1ui(m_animation, GLuint(a));
}

void ShaderCubeMap::loadUniforms()
{
	m_animation = locateUniform("animation");
}
