#pragma once
#include "../../Utility/Point.h"

class WindowDesc
{
	friend class WindowManager;
private:
	WindowDesc(WindowManager* wm, size_t anchor, PointF offset)
		:
		m_windowManager(wm),
		m_anchor(anchor),
		m_offset(offset)
	{}

public:
	virtual ~WindowDesc()
	{}

public:
	WindowManager* getWindowManager() const
	{
		return m_windowManager;
	}

private:
	WindowManager* m_windowManager = nullptr;
	size_t m_anchor = 0;
	PointF m_offset;
};
