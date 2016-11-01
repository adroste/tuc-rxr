#pragma once
#include "CubeDesc.h"
// Base class of Cube

class CubeBase
{
public:
	CubeBase(const CubeDesc& cd);
	virtual ~CubeBase();

	const CubeDesc& getDesc() const;
	// 0.0f = translucent block | 1.0f = solid block
	virtual float getShadow()
	{
		return 1.0f;
	}
private:
	CubeDesc m_cd;
};