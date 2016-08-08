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
		UIWindow::addUIObject(&m_btnCancel);
	}

	virtual ~UIWindow() override
	{}

	virtual void update(float dt) override
	{
		if (m_btnCancel.isClicked(true))
			hide();
	}

	virtual bool keyUp(SDL_Scancode s) override
	{
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
