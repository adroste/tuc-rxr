#pragma once
#include <cstdint>

enum class CubeShader
{
	Default,
	Special,
	Size
};

struct CubeDesc
{
	CubeDesc()
	{}
	CubeDesc(uint32_t diff)
		:
		diffuse(diff),
		spec(0),
		gloss(1.0f),
		shader(CubeShader::Default),
		glowing(0.0f)
	{}

	uint32_t diffuse;
	uint32_t spec;
	float gloss; // cos(phi)^gloss
	//uint16_t flags;
	CubeShader shader;
	//uint32_t special;
	float glowing;
};
