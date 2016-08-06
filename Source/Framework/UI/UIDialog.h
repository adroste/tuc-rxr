#pragma once
#include "UIWindow.h"

// UIDialogs are modal (=> blocking other input)
class UIDialog : public UIWindow
{
public:
	enum class Result
	{
		None,
		OK,
		Cancel,
		Yes,
		No,
		Retry,
		Ignore
	};

public:
	virtual ~UIDialog() override
	{
	}

	virtual void update(float dt) override
	{
		if (m_btnCancel.isClicked(true))
			setResult(Result::Cancel);

		// auto close
		if (m_autoClose && (
			m_dlgResult == Result::OK
			|| m_dlgResult == Result::Cancel
			|| m_dlgResult == Result::Yes
			|| m_dlgResult == Result::No
			|| m_dlgResult == Result::Ignore))
			hide();
	}

	virtual void draw(Drawing& draw) override
	{
		UIWindow::draw(draw);
	}

	// Input handling
	virtual bool keyDown(SDL_Scancode s) override
	{
		return true;
	}

	virtual bool keyUp(SDL_Scancode s) override
	{
		UIWindow::keyUp(s);

		if (s == SDL_SCANCODE_ESCAPE)
			setResult(Result::Cancel);

		return true;
	}

	virtual bool charDown(char c) override
	{
		return true;
	}

	virtual bool mouseDown(Input::Mouse button, const PointF& mpos) override
	{
		UIWindow::mouseDown(button, mpos);
		return true;
	}

	virtual bool mouseUp(Input::Mouse button, const PointF& mpos) override
	{
		UIWindow::mouseUp(button, mpos);
		return true;
	}

	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		UIWindow::mouseMove(mpos, handled);
		return true;
	}

	virtual bool wheel(float amount, const PointF& mpos) override
	{
		UIWindow::wheel(amount, mpos);
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
	}

private:
	Result m_dlgResult = Result::None;
	bool m_autoClose = true;
};
