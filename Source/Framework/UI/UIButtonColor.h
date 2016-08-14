#pragma once
#include "UIButton.h"
#include "Interfaces/IValueHolder.h"

class UIButtonColor : public UIButton, public IValueHolder<Color>
{
public:
	UIButtonColor(const Color& c)
		: UIButton(Style::Royal)
	{
		IValueHolder::setValue(c);
	}
	virtual void draw(Drawing& draw) override
	{
		// TODO replace with fancy button
		draw.rect(getRect(), getValue());
	}
};
