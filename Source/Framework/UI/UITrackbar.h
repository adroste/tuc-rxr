#pragma once
#include "UIObject.h"
#include "Interfaces/ISelectable.h"
#include "Interfaces/IValueHolder.h"
#include "../Utility/Tools.h"

class UITrackbar : public UIObject, public ISelectable, public IValueHolder<float>
{
	static const int THICK_BAR = 4;
	static const int THICK_SLIDER = 8;
public:
	// step = 0.0f means fluent
	UITrackbar(float val = 0.0f, float step = 0.0f)
		:
		IValueHolder(val),
		m_step(tool::clamp(step, 0.0f, 1.0f))
	{}

	virtual ~UITrackbar() override
	{}

	void draw(Drawing& draw) override
	{
		// TODO add proper trackbar
		//draw.rect(getRect(), Color::Cyan());
		draw.rect(getRectBar(), Color::Gray(0.6f));
		draw.rect(getRectSlider(), Color::Gray(0.8f));
	}

	RectF getRectBar() const
	{
		PointF dim = getDim();
		return RectF(0.0f, dim.y / 2 - float(THICK_BAR) / 2, dim.x, dim.y / 2 + float(THICK_BAR) / 2) + getOrigin();
	}

	RectF getRectSlider() const
	{
		PointF dim = getDim();
		float posSlider = (dim.x - float(THICK_SLIDER)) * getValue();
		return RectF(posSlider, 0.0f, posSlider + THICK_SLIDER, dim.y) + getOrigin();
	}

	// Input
	virtual bool keyUp(SDL_Scancode s) override
	{
		return false;
	}

	virtual bool mouseMove(const PointF& mpos, const PointF& mdiff, bool handled) override
	{
		if (m_isDragged)
		{
			float newVal = tool::clamp((mpos.x - getOrigin().x) / getDim().x, 0.0f, 1.0f);
			if (m_step != 0.0f)
				newVal = m_step * int((newVal / m_step) + 0.5f);
			if (newVal != getValue())
				setValue(newVal);
		}
		return false;
	}

	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		if (button == Input::Mouse::Left && getRectSlider().isPointInside(mpos))
		{
			m_isDragged = true;
			return true;
		}
		return false;
	}

	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		if (m_isDragged && button == Input::Mouse::Left)
		{
			m_isDragged = false;
			return true;
		}
		return false;
	}

	virtual bool wheel(const PointF& mpos, float amount) override
	{
		return false;
	}

	virtual void setValue(const float& value) override
	{
		IValueHolder::setValue(tool::clamp(value, 0.0f, 1.0f));
	}

private:
	float m_step;
	bool m_isDragged = false;
};