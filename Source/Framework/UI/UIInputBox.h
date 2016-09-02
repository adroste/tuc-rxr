#pragma once
#include "UIInputField.h"
#include "UIContainer.h"

class UIInputBox : public UIInputField
{
public:
	UIInputBox(Font& font, size_t maxlen)
		:
		UIInputField(font, maxlen)
	{}

	virtual void draw(Drawing& draw) override
	{
		// TODO replace with fancy box
		draw.rect(getRect(), Color::White());
		// shrink dimensions + move origin by 5.0f
		addPadding(PointF(5.0f)); 
		// draw InputField with shrinked dimensions + moved origin
		UIInputField::draw(draw);
		// reset dimensions + origin
		addPadding(PointF(-5.0f)); 
	}
};
