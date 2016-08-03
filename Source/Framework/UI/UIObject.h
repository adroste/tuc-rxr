#pragma once
#include "../OpenGL/IDrawable.h"
#include "Interfaces/IMetrics.h"

class UIObject : public IDrawable, public IMetrics
{
public:
	virtual ~UIObject() override
	{}

	virtual void setVisibility(bool visible)
	{
		m_isVisible = visible;
	}

	bool isVisible() const
	{
		return m_isVisible;
	}

private:
	bool m_isVisible = true;
};