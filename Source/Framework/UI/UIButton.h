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
	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		if (m_isHover)
			m_isDown = true;
		return m_isDown;
	}
	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		bool ret = false;
		if (m_isHover && m_isDown)
		{
			setClicked(true);
			ret = true;
		}
		m_isDown = false;
		return ret;
	}
	virtual bool keyDown(SDL_Scancode s) override
	{
		if (s == SDL_SCANCODE_RETURN && isSelected())
			m_isDown = true;
		return m_isDown;
	}
	virtual bool keyUp(SDL_Scancode s) override
	{
		bool ret = false;
		if (s == SDL_SCANCODE_RETURN && isSelected() && m_isDown)
		{
			setClicked(true);
			ret = true;
		}
		m_isDown = false;
		return ret;
	}

protected:
	bool m_isHover = false;
	bool m_isDown = false;

	Style m_style;
};
