#pragma once
#include "UIObject.h"
#include "../../System/Input.h"
#include <assert.h>

class UIContainer : public UIObject, public Input::IBroadcaster
{
public:
	UIContainer(bool show)
	{
		//setOrigin({ 0.0f, 0.0f });
		show ? UIObject::show() : UIObject::hide();
	}

	virtual ~UIContainer() override
	{}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		// TODO draw border, background
		// just testing
		pushDrawTransforms(draw, PointF(0.0f));
		RectF rct(PointF(0.0f), getDim());
		draw.rect(rct, Color::DarkGray());
		popDrawTransforms(draw);
	}

	// Input
	virtual bool keyDown(SDL_Scancode s) override
	{
		return sendKeyDown(s);
	}
	virtual bool keyUp(SDL_Scancode s) override
	{
		return sendKeyUp(s);
	}
	virtual bool charDown(char c) override
	{
		return sendCharDown(c);
	}
	virtual bool mouseMove(const PointF& mpos, const PointF& mdiff, bool handled) override
	{
		bool ret = sendMouseMove(mpos, mdiff, handled);
		m_isMouseInside = getRect().isPointInside(mpos);
		return m_isMouseInside || ret;
	}
	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		bool ret = sendMouseDown(mpos, button);
		return m_isMouseInside || ret;
	}
	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		bool ret = sendMouseUp(mpos, button);
		return m_isMouseInside || ret;
	}
	virtual bool wheel(const PointF& mpos, float amount) override
	{
		bool ret = sendWheel(mpos, amount);
		return m_isMouseInside || ret;
	}


	virtual void setDim(const PointF& d) override
	{
		assert(!m_fixedDim);
		if (!m_fixedDim)
			UIObject::setDim(d);
	}

	virtual void setOrigin(const PointF& p) override
	{
		IMetrics::setOrigin(p);
		m_matTransform = glm::translate(glm::vec3(p.x, p.y, 0.0f));
	}

	bool hasFixedDim() const
	{
		return m_fixedDim;
	}
protected:
	virtual PointF transformMouse(PointF mpos) override
	{
		return mpos - m_pos;
	}

	/*
	*	clipPadding: shrinks clipRect if clipPadding > 0, grows if < 0
	*				x - left / right padding
	*				y - top / bottom padding
	*	default value = border size
	*/
	virtual void pushDrawTransforms(Drawing& draw, PointF clipPadding = PointF(2.0f))
	{
		draw.getTransform().pushModel(m_matTransform);
		draw.pushIgnoreRect();
		draw.pushClippingRect(RectF(clipPadding, getDim() - clipPadding));
	}

	virtual void popDrawTransforms(Drawing& draw)
	{
		draw.popClippingRect();
		draw.popClippingRect();
		draw.getTransform().popModel();
	}

	void setFixedDim(bool fixedDim) // should be set inside constructor of derived classes
	{
		m_fixedDim = fixedDim;
	}

protected:
	glm::mat4 m_matTransform;
	bool m_isMouseInside = false;

private:
	bool m_fixedDim = false;
};
