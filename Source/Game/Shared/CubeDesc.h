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

	// visual part
	uint32_t diffuse;
	uint32_t spec;
	float gloss; // cos(phi)^gloss
	CubeShader shader;
	float glowing;

	// game part
	enum BlockType
	{
		Immortal,
		Transparent,
		Solid,
		Liquid
	};
	enum BlockFlag
	{
		Gravity
	};
	uint8_t blockType;
	uint8_t blockFlags;
	uint32_t blockHP; // 0 = not destroyable
};
