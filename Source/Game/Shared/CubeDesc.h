#pragma once
#include <cstdint>
#include <string>
#include "../../Utility/EnumConverter.h"

enum class CubeShader
{
	Default,
	Special,
	Size
};
enum class BlockType
{
	Immortal,
	Transparent,
	Solid,
	Liquid
};

ENUM_CONVERT_FUNC(CubeShader);
ENUM_CONVERT_FUNC(BlockType);

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

	enum BlockFlag
	{
		Gravity = 1
	};
	uint8_t blockType;
	uint8_t blockFlags;
	uint32_t blockHP; // 0 = not destroyable
};

