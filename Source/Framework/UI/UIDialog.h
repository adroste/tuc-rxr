#pragma once
#include "UIWindow.h"

// UIDialogs are modal (=> blocking other input)
class UIDialog : public UIWindow
{
	std::function<void(UIDialog*)> m_onResult = [](UIDialog*) {};
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
	virtual ~UIDialog() override
	{
	}

	void setOnResultCallback(decltype(m_onResult) c)
	{
		assert(c != nullptr);
		m_onResult = c;
	}

	virtual void update(float dt) override
	{
		if (m_btnCancel.isClicked(true))
			setResult(Result::Cancel);

		// auto close
		if (m_autoClose && m_dlgResult != Result::None)
			hide();
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
		UIWindow::mouseDown(mpos, button);
		return true;
	}

	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		UIWindow::mouseUp(mpos, button);
		return true;
	}

	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		UIWindow::mouseMove(mpos, handled);
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
		m_onResult(this);
	}

private:
	Result m_dlgResult = Result::None;
	bool m_autoClose = true;
};
