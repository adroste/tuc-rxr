#pragma once
#include "../IMetrics.h"
#include "Drawing.h"

class IDrawable : public IMetrics
{
public:
	virtual ~IDrawable() {}

	virtual void draw(Drawing& draw) = 0;
};