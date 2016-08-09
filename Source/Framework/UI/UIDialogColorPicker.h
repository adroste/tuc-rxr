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
		if (!isVisible()) return;
		
		UIDialog::draw(draw);

		pushDrawTransform(draw);
		float r = getDim().x / 2.0f;
		draw.hsvPicker({ r, r }, r, Color::Red());
		popDrawTransform(draw);
	}



private:
	float m_angle;
};
