#pragma once
#include "UIDialog.h"
#include "UITextBox.h"

class UIMessageBox : public UIDialog, public ILabelable
{
public:
	UIMessageBox(const std::string& txt = "", UIDialog::Buttons buttons = Buttons::OK)
		:
		UIDialog(buttons),
		m_txtBox(Drawing::getFont(Font::Style::Text, Font::Size::M))
	{
		UIMessageBox::setText(txt);
		UIMessageBox::setDim({ 600.0f, 300.0f });
	}

	virtual ~UIMessageBox() override
	{}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		UIDialog::draw(draw);
		pushDrawTransforms(draw);
		m_txtBox.draw(draw);
		popDrawTransforms(draw);
	}

	using UIDialog::show;
	virtual void show(const std::string& txt, UIDialog::Buttons buttons = Buttons::OK)
	{
		setText(txt);
		setButtons(buttons);
		show();
	}

	virtual void setDim(const PointF& d) override
	{
		UIDialog::setDim(d);
		m_txtBox.setDim({ d.x - 40.0f, d.y - 20.0f - (getBottomContentHeight() + 20.0f) });
		m_txtBox.setOrigin({ 20.0f, 20.0f });
	}
	
	virtual void setText(const std::string& text) override
	{
		ILabelable::setText(text);
		m_txtBox.setText(text);
	}

protected:
	UITextBox m_txtBox;
};
