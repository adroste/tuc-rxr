#pragma once
#ifdef _CLIENT
#include "Drawing.h"
#endif // _CLIENT

class IDrawable
{
#ifdef _CLIENT
public:
	virtual ~IDrawable() {}

	virtual void draw(Drawing& draw) = 0;
#endif // _CLIENT
};
