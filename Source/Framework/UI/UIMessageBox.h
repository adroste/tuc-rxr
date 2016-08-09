#pragma once
#include "UIDialog.h"
#include "UITextBox.h"

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
		m_txtBox(Drawing::getFont(Font::Style::Text, Font::Size::M)),
		m_btn1(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Cancel"),
		m_btn2(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S)),
		m_btn3(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S))
	{
		UIMessageBox::setText(txt);

		m_btn1.adjustToFontHeadline();
		m_btn2.setDim(m_btn1.getDim());
		m_btn3.setDim(m_btn1.getDim());
		
		UIMessageBox::setDim({ 600.0f, 300.0f });

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

	virtual void update(float dt) override
	{
		UIDialog::update(dt);

		if (m_btn1.isClicked(true))
			setResult(m_resBtn1);
		if (m_btn2.isClicked(true))
			setResult(m_resBtn2);
		if (m_btn3.isClicked(true))
			setResult(m_resBtn3);		
	}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		UIDialog::draw(draw);
		pushDrawTransform(draw);
		m_txtBox.draw(draw);
		m_btn1.draw(draw);
		m_btn2.draw(draw);
		m_btn3.draw(draw);
		popDrawTransform(draw);
	}

	using UIDialog::show;
	virtual void show(const std::string& txt, UIMessageBox::Buttons buttons = Buttons::OK)
	{
		setText(txt);
		m_buttons = buttons;
		init();
		show();
	}

	virtual void setDim(const PointF& d) override
	{
		UIDialog::setDim(d);
		m_txtBox.setDim({ d.x - 40.0f, d.y - 20.0f - (m_btn1.getDim().y + 20.0f) });
		m_txtBox.setOrigin({ 20.0f, 20.0f });
		m_btn1.setOrigin({ d.x - (m_btn1.getDim().x + 10.0f), d.y - (m_btn1.getDim().y + 10.0f) });
		m_btn2.setOrigin({ m_btn1.getOrigin().x - (m_btn2.getDim().x + 10.0f), d.y - (m_btn2.getDim().y + 10.0f) });
		m_btn3.setOrigin({ m_btn2.getOrigin().x - (m_btn3.getDim().x + 10.0f), d.y - (m_btn3.getDim().y + 10.0f) });
	}
	
	virtual void setText(const std::string& text) override
	{
		ILabelable::setText(text);
		m_txtBox.setText(text);
	}

protected:
	virtual void init()
	{
		// TODO preselect buttons (to just press enter for default behaviour)
		switch (m_buttons)
		{
		case Buttons::OK: 
			m_btn1.setText("OK");
			m_resBtn1 = Result::OK;
			m_btn1.show();
			m_resBtn2 = Result::None;
			m_btn2.hide();
			m_resBtn3 = Result::None;
			m_btn3.hide();
			break;
		case Buttons::OKCancel: 
			m_btn1.setText("Cancel");
			m_resBtn1 = Result::Cancel;
			m_btn1.show();
			m_btn2.setText("OK");
			m_resBtn2 = Result::OK;
			m_btn2.show();
			m_resBtn3 = Result::None;
			m_btn3.hide();
			break;
		case Buttons::YesNo: 
			m_btn1.setText("No");
			m_resBtn1 = Result::No;
			m_btn1.show();
			m_btn2.setText("Yes");
			m_resBtn2 = Result::Yes;
			m_btn2.show();
			m_resBtn3 = Result::None;
			m_btn3.hide();
			break;
		case Buttons::AbortRetryIgnore: 
			m_btn1.setText("Ignore");
			m_resBtn1 = Result::Ignore;
			m_btn1.show();
			m_btn2.setText("Retry");
			m_resBtn2 = Result::Retry;
			m_btn2.show();
			m_btn3.setText("Abort");
			m_resBtn3 = Result::Cancel;
			m_btn3.show();
			break;
		default: 
			throw ExceptionInvalidOperation("UIMessageBox::init default case", "m_buttons invalid");
		}
	}

protected:
	Buttons m_buttons;

	UITextBox m_txtBox;
	UIButtonText m_btn1;
	UIButtonText m_btn2;
	UIButtonText m_btn3;

	UIDialog::Result m_resBtn1 = Result::None;
	UIDialog::Result m_resBtn2 = Result::None;
	UIDialog::Result m_resBtn3 = Result::None;
};
