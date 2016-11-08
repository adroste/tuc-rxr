#pragma once
#include "CubeDesc.h"
// Base class of Cube

class CubeBase
{
public:
	enum Side
	{
		Left = 1,
		Right = 2,
		Top = 4,
		Bottom = 8,
		Front = 16,
		Back = 32
	};
public:
	CubeBase(const CubeDesc& cd);
	virtual ~CubeBase();

	const CubeDesc& getDesc() const;
	// 0.0f = translucent block | 1.0f = solid block
	virtual float getShadow()
	{
		if(m_cd.shader == CubeShader::Default)
			return 1.0f;
		return 0.3f;
	}
	bool isOpaque() const;
	bool hasTransparency() const;
public:
	// indicates if there are neighboring blocks in this chunk
	size_t neighbors = 0;

private:
	CubeDesc m_cd;
};