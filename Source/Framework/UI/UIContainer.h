#pragma once
#include "UIObject.h"
#include "../../System/Input.h"
#include <assert.h>

class UIContainer : public UIObject, public Input::IBroadcaster
{
public:
	UIContainer(bool show = false)
	{
		show ? UIObject::show() : UIObject::hide();
	}

	virtual ~UIContainer() override
	{}

	virtual void update(float dt)
	{}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		// TODO draw border, background
		// just testing
		{
			draw.rect(getRect(), Color::Gray());
			draw.rect(getRect().shrink(2.0f), Color::DarkGray());
		}
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
	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		sendMouseMove(mpos, handled);
		m_isMouseInside = getRect().isPointInside(mpos);
		return m_isMouseInside;
	}
	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		sendMouseDown(mpos, button);
		return m_isMouseInside;
	}
	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		sendMouseUp(mpos, button);
		return m_isMouseInside;
	}
	virtual bool wheel(const PointF& mpos, float amount) override
	{
		sendWheel(mpos, amount);
		return m_isMouseInside;
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
		for (auto r : m_receivers)
			r->setInpTransform(getInpTransform() * glm::translate(glm::vec3(-p.x, -p.y, 0.0f)));
	}

	virtual void setInpTransform(glm::mat4 transform) override
	{
		UIObject::setInpTransform(transform);
		for (auto r : m_receivers)
			r->setInpTransform(transform * r->getInpTransform());
	}

protected:
	virtual void pushDrawTransform(Drawing& draw)
	{
		draw.getTransform().pushModel(m_matTransform);
	}

	virtual void popDrawTransform(Drawing& draw)
	{
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
