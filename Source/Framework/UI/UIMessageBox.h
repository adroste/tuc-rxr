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
	UIMessageBox(const std::string& txt = "", UIMessageBox::Buttons buttons = Buttons::OK)
		:
		m_buttons(buttons),
		m_btn1(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Cancel"),
		m_btn2(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S)),
		m_btn3(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S))
	{
		ILabelable::setText(txt);

		m_btn1.adjustToFontHeadline();
		m_btn2.setDim(m_btn1.getDim());
		m_btn3.setDim(m_btn1.getDim());
		
		m_btn1.registerMe(this);
		m_btn2.registerMe(this);
		m_btn3.registerMe(this);

		UIMessageBox::init();
	}

	virtual ~UIMessageBox() override
	{
		m_btn1.unregisterMe();
		m_btn2.unregisterMe();
		m_btn3.unregisterMe();
	}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		UIDialog::draw(draw);
		pushDrawTransform(draw);
		m_btn1.draw(draw);
		m_btn2.draw(draw);
		m_btn3.draw(draw);
		popDrawTransform(draw);
	}

	using UIDialog::show;
	virtual void show(const std::string& txt, UIMessageBox::Buttons buttons)
	{
		setText(txt);
		m_buttons = buttons;
		init();
		show();
	}

	virtual void setDim(const PointF& d) override
	{
		UIDialog::setDim(d);
		m_btn1.setOrigin({ d.x - (m_btn1.getDim().x + 10.0f), d.y - (m_btn1.getDim().y + 10.0f) });
		m_btn2.setOrigin({ m_btn1.getOrigin().x - (m_btn2.getDim().x + 10.0f), d.y - (m_btn2.getDim().y + 10.0f) });
		m_btn3.setOrigin({ m_btn2.getOrigin().x - (m_btn3.getDim().x + 10.0f), d.y - (m_btn3.getDim().y + 10.0f) });
	}

protected:
	virtual void init()
	{
		switch (m_buttons)
		{
		case Buttons::OK: 
			m_btn1.setText("OK");
			m_btn1.show();
			m_btn2.hide();
			m_btn3.hide();
			break;
		case Buttons::OKCancel: 
			m_btn1.setText("Cancel");
			m_btn1.show();
			m_btn2.setText("OK");
			m_btn2.show();
			m_btn3.hide();
			break;
		case Buttons::YesNo: 
			m_btn1.setText("No");
			m_btn1.show();
			m_btn2.setText("Yes");
			m_btn2.show();
			m_btn3.hide();
			break;
		case Buttons::AbortRetryIgnore: 
			m_btn1.setText("Ignore");
			m_btn1.show();
			m_btn2.setText("Retry");
			m_btn2.show();
			m_btn3.setText("Abort");
			m_btn3.show();
			break;
		default: 
			throw ExceptionInvalidOperation("UIMessageBox::init default case", "m_buttons invalid");
		}
	}

protected:
	Buttons m_buttons;

	UIButtonText m_btn1;
	UIButtonText m_btn2;
	UIButtonText m_btn3;
};
