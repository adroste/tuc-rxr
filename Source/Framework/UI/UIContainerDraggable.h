#pragma once
#include "UIContainer.h"

class UIContainerDraggable : public UIContainer
{
public:
	UIContainerDraggable(bool show = false)
		:
		UIContainer(show)
	{}

	virtual ~UIContainerDraggable() override
	{}

	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		if (m_isMouseInside && button == Input::Mouse::Left)
		{
			m_isMouseLeftDown = true;
			m_dragSpot = mpos;
		}

		return m_isMouseInside;
	}

	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		if (button == Input::Mouse::Left)
			m_isMouseLeftDown = false;

		return m_isMouseInside;
	}

	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		m_isMouseInside = getRect().isPointInside(mpos);

		// drag window
		if (m_isMouseLeftDown)
		{
			setOrigin(getOrigin() + mpos - m_dragSpot);
			m_dragSpot = mpos;
		}

		return m_isMouseInside;
	}

protected:
	bool m_isMouseLeftDown = false;
	PointF m_dragSpot;
};