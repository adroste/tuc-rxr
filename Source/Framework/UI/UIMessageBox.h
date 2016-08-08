#pragma once
#include "UIDialog.h"

class UIMessageBox : public UIDialog, public ILabelable
{
public:
	enum class Buttons
	{
		OK,
		OKCancel,
		YesNo,
		AbortRetryIgnore
	};

public:
	UIMessageBox(UIMessageBox::Buttons buttons, const std::string& txt)
		:
		m_btn1(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), ""),
		m_btn2(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), ""),
		m_btn3(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "")
	{
		ILabelable::setText(txt);

		m_btn1.disable();
		m_btn2.disable();
		m_btn3.disable();
		m_btn1.setVisibility(false);
		m_btn2.setVisibility(false);
		m_btn3.setVisibility(false);

		switch (buttons)
		{
		case Buttons::OK:
			m_btn1.setText("OK");
			UIDialog::addUIObject(&m_btn1);
			break;
		case Buttons::OKCancel:
		}
	}

	virtual ~UIMessageBox() override
	{}

protected:
	UIButtonText m_btn1;
	UIButtonText m_btn2;
	UIButtonText m_btn3;
};
