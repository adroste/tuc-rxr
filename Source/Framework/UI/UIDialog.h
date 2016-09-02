#pragma once
#include "UIWindow.h"
#include "Callback.h"

// UIDialogs are modal (=> blocking other input)
class UIDialog : public UIWindow
{
	CALLBACK(Result, UIDialog*);
	CALLBACKPROTECTED(ResultProtected, UIDialog*);
public:
	enum class Result
	{
		None,
		OK,
		Cancel, // cancel == abort
		Yes,
		No,
		Retry,
		Ignore
	};

	enum class Buttons
	{
		None,
		OK,
		OKCancel,
		YesNo,
		AbortRetryIgnore
	};

public:
	UIDialog(Buttons buttons = Buttons::None)
		:
		m_btn1(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S)),
		m_btn2(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S)),
		m_btn3(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S))
	{
		// set equal button dimensions
		m_btn1.setText("Cancel");
		m_btn1.adjustToFontHeadline();
		m_btn2.setDim(m_btn1.getDim());
		m_btn3.setDim(m_btn1.getDim());

		UIDialog::setButtons(buttons);

		m_btn1.registerMe(this);
		m_btn2.registerMe(this);
		m_btn3.registerMe(this);

		m_btn1.setOnClickCallback([this](IClickable*)
		{
			setResult(m_resBtn1);
		});
		m_btn2.setOnClickCallback([this](IClickable*)
		{
			setResult(m_resBtn2);
		});
		m_btn3.setOnClickCallback([this](IClickable*)
		{
			setResult(m_resBtn3);
		});

		// button "X" at top-right corner
		m_btnCancel.setOnClickCallback([this](IClickable*)
		{
			setResult(Result::Cancel);
		});
	}

	virtual ~UIDialog() override
	{
		m_btn1.unregisterMe();
		m_btn2.unregisterMe();
		m_btn3.unregisterMe();
	}
	
	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		UIWindow::draw(draw);
		pushDrawTransform(draw);
		m_btn1.draw(draw);
		m_btn2.draw(draw);
		m_btn3.draw(draw);
		popDrawTransform(draw);
	}

	// Input handling
	virtual bool keyDown(SDL_Scancode s) override
	{
		UIWindow::keyDown(s);
		return true;
	}

	virtual bool keyUp(SDL_Scancode s) override
	{
		/*UIWindow::keyUp(s);

		if (s == SDL_SCANCODE_ESCAPE)
			setResult(Result::Cancel);

		return true;*/

		bool handled = sendKeyUp(s);

		if (!handled)
		{
			if (s == SDL_SCANCODE_ESCAPE)
			{
				hide();
				setResult(Result::Cancel);
			}
		}
		return true;
	}

	virtual bool charDown(char c) override
	{
		UIWindow::charDown(c);
		return true;
	}

	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		if (!getRect().isPointInside(mpos))
			Sound::playSound(Sound::Sfx::Knock);

		UIWindow::mouseDown(mpos, button);
		return true;
	}

	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		UIWindow::mouseUp(mpos, button);
		return true;
	}

	virtual bool mouseMove(const PointF& mpos, const PointF& mdiff, bool handled) override
	{
		UIWindow::mouseMove(mpos, mdiff, handled);
		return true;
	}

	virtual bool wheel(const PointF& mpos, float amount) override
	{
		UIWindow::wheel(mpos, amount);
		return true;
	}


	virtual void show() override
	{
		UIWindow::show();
		setResult(Result::None);
	}

	virtual void setDim(const PointF& d) override
	{
		UIWindow::setDim(d);
		m_btn1.setOrigin({ d.x - (m_btn1.getDim().x + 10.0f), d.y - (m_btn1.getDim().y + 10.0f) });
		m_btn2.setOrigin({ m_btn1.getOrigin().x - (m_btn2.getDim().x + 10.0f), d.y - (m_btn2.getDim().y + 10.0f) });
		m_btn3.setOrigin({ m_btn2.getOrigin().x - (m_btn3.getDim().x + 10.0f), d.y - (m_btn3.getDim().y + 10.0f) });
	}

	Result getResult() const
	{
		return m_dlgResult;
	}

	void setAutoClose(bool autoClose)
	{
		m_autoClose = autoClose;
	}

	virtual void setButtons(UIDialog::Buttons buttons)
	{
		// TODO preselect buttons (to just press enter for default behaviour)
		switch (buttons)
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
		case Buttons::None:
		default:
			m_resBtn1 = Result::None;
			m_btn1.hide();
			m_resBtn2 = Result::None;
			m_btn2.hide();
			m_resBtn3 = Result::None;
			m_btn3.hide();
			break;
		}
	}

protected:
	void setResult(Result dlgResult)
	{
		m_dlgResult = dlgResult;
		if (m_dlgResult != Result::None)
		{
			m_onResultProtected(this);
			if (m_autoClose)
				hide();
			m_onResult(this);
		}
	}

	// returns space hold by buttons
	float getBottomContentHeight() const
	{
		// all buttons should have the same size
		return m_btn1.getDim().y + 10.0f;
	}

private:
	Result m_dlgResult = Result::None;
	bool m_autoClose = true;

	// Buttons
	UIButtonText m_btn1;
	UIButtonText m_btn2;
	UIButtonText m_btn3;
	Result m_resBtn1 = Result::None;
	Result m_resBtn2 = Result::None;
	Result m_resBtn3 = Result::None;
};
