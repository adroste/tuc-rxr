#pragma once
#include "UIDialog.h"
#include "UIColorPicker.h"

class UIDialogColorPicker : public UIDialog
{
public:
	UIDialogColorPicker()
	{
		UIDialog::setDim({ 300.0f, 450.0f });
		setFixedDim(true);

		m_colorPicker.registerMe(this);
	}

	virtual ~UIDialogColorPicker() override
	{
		m_colorPicker.unregisterMe();
	}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;
		
		UIDialog::draw(draw);

		pushDrawTransform(draw);
		m_colorPicker.draw(draw);
		popDrawTransform(draw);
	}


	Color getColor() const
	{
		return m_colorPicker.getColor();
	}


private:
	UIColorPicker m_colorPicker;
};
