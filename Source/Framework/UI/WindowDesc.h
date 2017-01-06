#pragma once
#include "../../Utility/Point.h"

class WindowDesc
{
	friend class WindowManager;
private:
	WindowDesc(class WindowManager* wm, size_t anchor, PointF offset, bool isDialog)
		:
		m_windowManager(wm),
		m_anchor(anchor),
		m_offset(offset),
		m_isDialog(isDialog)
	{}

public:
	virtual ~WindowDesc()
	{}

public:
	class WindowManager* getWindowManager() const
	{
		return m_windowManager;
	}

	bool isDialog() const
	{
		return m_isDialog;
	}

private:
	class WindowManager* m_windowManager = nullptr;
	size_t m_anchor = 0;
	PointF m_offset;
	bool m_isDialog;
};
