#pragma once
#include "UIObject.h"
#include "Interfaces/IClickable.h"
#include "Callback.h"

class UICheckBox : public UIObject, public IClickable, public IValueHolder<bool>
{
public:
	UICheckBox(bool isChecked = false)
		:
		IValueHolder(isChecked)
	{
		UICheckBox::setDim(PointF(30.0f));		
	}

	void draw(Drawing& draw) override
	{
		RectF rct(getOrigin(), getOrigin() + getDim());
		draw.rect(rct, Color::Gray(0.8f));
		draw.rect(rct.shrink(5.0f), Color::Gray(0.2f));
		if (getValue())
			draw.rect(rct.shrink(7.0f), Color::White());
	}


	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		if (!getRect().isPointInside(mpos))
			return false;
		
		if (button == Input::Mouse::Left)
			m_isMouseDown = true;
		return true;
	}

	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		if (!getRect().isPointInside(mpos)) 
			return false;

		if (m_isMouseDown && button == Input::Mouse::Left)
		{
			m_isMouseDown = false;
			setClicked(true);
			setValue(!getValue());
		}		
		return true;
	}

private:
	bool m_isMouseDown = false;
};
