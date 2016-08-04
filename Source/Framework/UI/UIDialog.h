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
	{}

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

protected:
	void setResult(Result dlgResult)
	{
		m_dlgResult = dlgResult;
	}

private:
	Result m_dlgResult = Result::None;
};
