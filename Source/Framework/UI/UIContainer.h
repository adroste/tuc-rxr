#pragma once
#include "UIObject.h"
#include "../../System/Input.h"
#include "../../System/Exceptions/ExceptionInvalidOperation.h"

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

	/*virtual void setVisibility(bool visible) override
	{
	UIObject::setVisibility(visible);
	for (auto o : m_uiObjects)
	o->setVisibility(visible);
	}
	do not use: ui objects visible status should reveal if objects are visible INSIDE the container, not at all*/

protected:
	// every container object needs to be added with this function
	/*virtual void addUIObject(UIObject* obj)
	{
		m_uiObjects.push_back(obj);
		Input::IReceiver* pRec = dynamic_cast<Input::IReceiver*>(obj);
		if (pRec)
		{
			if (pRec->getZIndex() <= getZIndex())
				throw ExceptionInvalidOperation("UIWindow::addUIObject", "z-index of object is <= z-index of window");
			m_receivers.push_back(pRec);
		}
		m_isShown ? enable() : disable();
	}*/

	virtual void pushDrawTransform(Drawing& draw)
	{
		draw.getTransform().pushModel(m_matTransform);
	}

	virtual void popDrawTransform(Drawing& draw)
	{
		draw.getTransform().popModel();
	}

protected:
	glm::mat4 m_matTransform;
	bool m_isMouseInside = false;
};
