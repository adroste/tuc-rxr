#pragma once
#include "../../Utility/Point.h"

class WindowDesc
{
	friend class WindowManager;
	friend class WindowLayer;
private:
	WindowDesc(WindowLayer* wl, size_t anchor, PointF offset)
		:
		m_windowLayer(wl),
		m_anchor(anchor),
		m_offset(offset)
	{}

public:
	virtual ~WindowDesc()
	{}

public:
	WindowLayer* getWindowLayer() const
	{
		return m_windowLayer;
	}

private:
	WindowLayer* m_windowLayer = nullptr;
	size_t m_anchor = 0;
	PointF m_offset;
};
