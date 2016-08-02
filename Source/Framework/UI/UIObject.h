#pragma once
#include "../OpenGL/IDrawable.h"
#include "Interfaces/IMetrics.h"

class UIObject : public IDrawable, public IMetrics
{
public:
	virtual ~UIObject() override
	{}
};