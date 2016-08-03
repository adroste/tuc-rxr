#pragma once
#include "../../System/Input.h"
#include "Interfaces/IClickable.h"
#include "UIObject.h"
#include "Interfaces/ISelectable.h"


class UIButton : public UIObject, public Input::IReceiver, public IClickable, public ISelectable
{
public:
	enum class Style
	{
		Royal
	};

public:
	UIButton(Style style) 
		:
		m_style(style)
	{}

	virtual ~UIButton()
	{}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		switch(m_style)
		{
		case Style::Royal:
			draw.buttonRoyal(getRect(), m_isDown);
		}
	}

	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		m_isHover = RectF(getRect()).isPointInside(mpos);
		// TODO ??? handled
		return handled;
	}
	virtual bool mouseDown(Input::Mouse button, const PointF& mpos) override
	{
		if (m_isHover)
			m_isDown = true;
		return m_isDown;
	}
	virtual bool mouseUp(Input::Mouse button, const PointF& mpos) override
	{
		if (m_isHover && m_isDown)
			setClicked(true);
		m_isDown = false;
		return isClicked(false);
	}
	virtual bool keyDown(SDL_Scancode s) override
	{
		if (s == SDL_SCANCODE_RETURN && isSelected())
			m_isDown = true;
		return m_isDown;
	}
	virtual bool keyUp(SDL_Scancode s) override
	{
		if (s == SDL_SCANCODE_RETURN && isSelected() && m_isDown)
			setClicked(true);
		m_isDown = false;
		return isClicked(false);
	}

protected:
	bool m_isHover = false;
	bool m_isDown = false;

	Style m_style;
};
