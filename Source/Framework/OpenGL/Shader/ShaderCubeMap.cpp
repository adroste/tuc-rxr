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

void ShaderCubeMap::setTransparent(bool isTrans)
{
	bind();
	glUniform1ui(m_transparent, GLuint(isTrans ? 1 : 0));
}

void ShaderCubeMap::loadUniforms()
{
	m_animation = locateUniform("animation");
	m_transparent = locateUniform("flipCoords");
}
