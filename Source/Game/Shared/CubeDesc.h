#pragma once
#include <cstdint>
#include <string>
#include "../../Utility/EnumConverter.h"

enum class CubeShader
{
	Default,
	Transparent,
	Water,
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
		shader(CubeShader::Default)
	{}

	// visual part
	uint32_t diffuse;
	uint32_t spec;
	float gloss; // cos(phi)^gloss
	CubeShader shader;

	enum BlockFlag
	{
		Gravity = 1,
		Glowing = 2
	};
	uint8_t blockType;
	uint8_t blockFlags;
	uint32_t blockHP; // 0 = not destroyable

	bool operator==(const CubeDesc& r) const
	{
		return memcmp(this, &r, sizeof(this)) == 0;
	}
	bool operator!=(const CubeDesc& r) const
	{
		return !this->operator==(r);
	}
	
	// ugly pink cube
	static CubeDesc getUgly()
	{
		return CubeDesc(0xD725DEFF);
	}
};

