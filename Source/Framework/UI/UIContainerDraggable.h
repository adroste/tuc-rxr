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
		bool handled = sendMouseDown(mpos, button);

		if (!handled)
		{
			if (m_isMouseInside && button == Input::Mouse::Left)
			{
				m_isDragged = true;
				m_dragSpot = mpos;
			}
		}

		return m_isMouseInside || handled;
	}

	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		bool handled = sendMouseUp(mpos, button);

		if (!handled)
		{
			if (button == Input::Mouse::Left)
				m_isDragged = false;
		}

		return m_isMouseInside || handled;
	}

	virtual bool mouseMove(const PointF& mpos, const PointF& mdiff, bool handled) override
	{
		handled = sendMouseMove(mpos, mdiff, handled);

		m_isMouseInside = getRect().isPointInside(mpos);

		if (!handled)
		{
			// drag window
			// TODO drag window on top border, not everywhere (wait for window-design)
			if (m_isDragged)
			{
				setOrigin(getOrigin() + mpos - m_dragSpot);
				m_dragSpot = mpos;
			}
		}

		return m_isMouseInside || handled;
	}

protected:
	bool m_isDragged = false;
	PointF m_dragSpot;
};