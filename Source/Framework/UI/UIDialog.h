#pragma once
#include "UIWindow.h"
#include "Callback.h"

// UIDialogs are modal (=> blocking other input)
class UIDialog : public UIWindow
{
	CALLBACK(Result, UIDialog*);
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

public:
	UIDialog()
	{
		m_btnCancel.setOnClickCallback([this](IClickable*)
		{
			setResult(Result::Cancel);
		});
	}

	virtual ~UIDialog() override
	{
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

	Result getResult() const
	{
		return m_dlgResult;
	}

	void setAutoClose(bool autoClose)
	{
		m_autoClose = autoClose;
	}

protected:
	void setResult(Result dlgResult)
	{
		m_dlgResult = dlgResult;
		if (m_dlgResult != Result::None)
		{
			if (m_autoClose)
				hide();
			m_onResult(this);
		}
	}

private:
	Result m_dlgResult = Result::None;
	bool m_autoClose = true;
};
