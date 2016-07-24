#pragma once
#include "../OpenGL/IDrawable.h"
#include "Interfaces/IMetrics.h"
#include "../../System/Input.h"
#include "Interfaces/IClickable.h"


class UIButton : public IDrawable, public IMetrics, public Input::IReceiver, public IClickable
{
public:
	virtual ~UIButton() {}

	void draw(Drawing& draw) override
	{
		static float border = 5.0f;
		//draw.rect(getRect(), Color::White());
		//draw.rect(getRect().shrink(border), m_isHover ? Color::Cyan() : Color::Black());
		draw.button(getRect(), border);
	}

	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		m_isHover = RectF(getRect()).isPointInside(mpos);
		// TODO ??? handled
		return handled;
	}
	virtual bool mouseDown(Input::Mouse button, const PointF& mpos) override
	{
		if (m_isHover)
			m_isDown = true;
		return m_isDown;
	}
	virtual bool mouseUp(Input::Mouse button, const PointF& mpos) override
	{
		if (m_isHover && m_isDown)
			setClicked(true);
		m_isDown = false;
		return isClicked(false);
	}

protected:
	bool m_isHover = false;
	bool m_isDown = false;
};
