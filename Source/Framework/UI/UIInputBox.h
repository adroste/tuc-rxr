#pragma once
#include "UIInputField.h"
#include "UIContainer.h"

class UIInputBox : public UIInputField
{
public:
	UIInputBox(Font& font, size_t maxlen)
		:
		UIInputField(font, maxlen)
	{
		setPadding({ 5.0f, 5.0f });
	}

	virtual void draw(Drawing& draw) override
	{
		// TODO replace with fancy box
		draw.rect(getRect(), Color::White());
		UIInputField::draw(draw);
	}
};
