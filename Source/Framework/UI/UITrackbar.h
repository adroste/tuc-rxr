#pragma once
#include "UIObject.h"
#include "Interfaces/ISelectable.h"
#include "Interfaces/IValueHolder.h"
#include "../Utility/Tools.h"

class UITrackbar : public UIObject, public ISelectable, public IValueHolder<float>
{
public:
	UITrackbar(float val = 0.0f)
		:
		IValueHolder(val)
	{}

	virtual ~UITrackbar() override
	{}

	void draw(Drawing& draw) override
	{
		// TODO add proper trackbar
		PointF dim = getDim();
		float thickness = 2.0f; // thickness / 2
		draw.rect(RectF(0.0f, dim.y / 2 - thickness, 0.0f, dim.y / 2 + thickness), Color::Gray(0.6f));
		float width = dim.x * getValue();
		draw.rect(RectF(width - thickness, 0.0f, width + thickness, 0.0f), Color::Gray(0.8f));
	}

	// Input
	virtual bool keyUp(SDL_Scancode s) override;

	virtual bool mouseMove(const PointF& mpos, const PointF& mdiff, bool handled) override;

	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override;

	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override;

	virtual bool wheel(const PointF& mpos, float amount) override;

	virtual void setValue(const float& value) override
	{
		setValue(tool::clamp(value, 0.0f, 1.0f));
	}
};