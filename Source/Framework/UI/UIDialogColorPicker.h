#pragma once
#include "UIDialog.h"

class UIDialogColorPicker : public UIDialog
{
public:
	UIDialogColorPicker()
	{
		UIDialog::setDim({ 300.0f, 450.0f });
		setFixedDim(true);
	}

	virtual ~UIDialogColorPicker() override
	{}

	virtual void draw(Drawing& draw) override
	{
		UIDialog::draw(draw);

		draw.hsvPicker(getMidpoint(), getDim().x / 2.0f, Color::Red());
	}



private:
	float m_angle;
};
