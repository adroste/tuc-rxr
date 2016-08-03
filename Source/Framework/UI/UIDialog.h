#pragma once
#include "UIWindow.h"

// UIDialogs are modal (=> blocking other input)
class UIDialog : public UIWindow
{
public:
	virtual ~UIDialog() override;


	virtual bool keyDown(SDL_Scancode s) override
	{
		return true;
	}
	virtual bool keyUp(SDL_Scancode s) override
	{
		return true;
	}
	virtual bool charDown(char c) override
	{
		return true;
	}
	virtual bool wheel(float amount, const PointF& mpos) override
	{
		return true;
	}
	virtual bool mouseDown(Input::Mouse button, const PointF& mpos) override
	{
		return true;
	}
	virtual bool mouseUp(Input::Mouse button, const PointF& mpos) override
	{
		return true;
	}
	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		return true;
	}
};
