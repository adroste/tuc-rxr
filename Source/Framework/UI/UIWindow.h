#pragma once
#include "UIContainerDraggable.h"
#include "UIButtonText.h"

class UIWindow : public UIContainerDraggable
{
public:
	UIWindow(bool show = false)
		:
		UIContainerDraggable(show),
		m_btnCancel(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "X")
	{
		m_btnCancel.setZIndex(getZIndex() + 1);
		m_btnCancel.adjustToFontHeadline();
		m_btnCancel.registerMe(this);
	}

	virtual ~UIWindow() override
	{
		m_btnCancel.unregisterMe();
	}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		UIContainerDraggable::draw(draw);
		pushDrawTransform(draw);
		m_btnCancel.draw(draw);
		popDrawTransform(draw);
	}

	virtual void update(float dt) override
	{
		if (m_btnCancel.isClicked(true))
			hide();
	}

	virtual bool keyUp(SDL_Scancode s) override
	{
		sendKeyUp(s);

		if (s == SDL_SCANCODE_ESCAPE)
		{
			hide();
			return true;
		}
		return false;
	}

	virtual void setDim(const PointF& d) override
	{
		IMetrics::setDim(d);
		m_btnCancel.setOrigin({ d.x - m_btnCancel.getDim().x + 30.0f, -20.0f });
	}

protected:	
	UIButtonText m_btnCancel;
};
