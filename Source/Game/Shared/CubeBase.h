#pragma once
// Base class of Cube

class CubeBase
{
public:
	virtual ~CubeBase()
	{
	}

	// return 0 for no material
	virtual size_t getMaterialIndex()
	{
		return 0;
	}
	// 0.0f = translucent block | 1.0f = solid block
	virtual float getShadow()
	{
		return 1.0f;
	}
};