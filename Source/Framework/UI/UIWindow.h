#pragma once
#include "UIObject.h"
#include "../../System/Input.h"
#include "../GameState.h";

class UIWindow : public UIObject, public Input::IReceiver
{
public:
	virtual ~UIWindow() override
	{
		if (m_parent)
		{
			unregisterMe(m_parent);
			m_parent = nullptr;
		}
	}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		// TODO draw border, background, window-buttons
		// just testing
		{
			draw.rect(getRect(), Color::Gray());
			draw.rect(getRect().shrink(2.0f), Color::DarkGray());
		}

		for (auto obj : m_uiObjects)
		{
			obj->draw(draw);
		}
	}

	virtual bool mouseDown(Input::Mouse button, const PointF& mpos) override
	{
		if (m_isMouseInside && button == Input::Mouse::Left)
		{
			m_isMouseLeftDown = true;
			m_dragSpot = mpos;
		}

		return m_isMouseInside;
	}

	virtual bool mouseUp(Input::Mouse button, const PointF& mpos) override
	{
		if (button == Input::Mouse::Left)
			m_isMouseLeftDown = false;

		return m_isMouseInside;
	}

	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		m_isMouseInside = getRect().isPointInside(mpos);

		// drag window
		if (m_isMouseLeftDown)
		{
			setOrigin(getOrigin() + mpos - m_dragSpot);
			m_dragSpot = mpos;			
		}

		return m_isMouseInside;
	}

	virtual bool wheel(float amount, const PointF& mpos) override
	{
		return m_isMouseInside;
	}

	// register after all UIObjects has been added
	virtual void registerMe(GameState* parent) override final
	{
		m_parent = parent;
		Input::IReceiver::registerMe(parent);
		for (auto r : m_receivers)
		{
			r->registerMe(parent);
		}
	}

	virtual void unregisterMe(GameState* parent) override final // do not remove this final!! 
	{
		for (auto r : m_receivers)
		{
			r->unregisterMe(parent);
		}
		Input::IReceiver::unregisterMe(parent);
	}

protected:
	virtual void addUIObject(UIObject* obj)
	{
		m_uiObjects.push_back(obj);
		Input::IReceiver* pRec = dynamic_cast<Input::IReceiver*>(obj);
		if (pRec)
		{
			if (pRec->getZIndex() <= getZIndex())
				throw ExceptionInvalidOperation("UIWindow::addUIObject", "z-index of object is <= z-index of window");
			m_receivers.push_back(pRec);
		}
	}

protected:
	bool m_isMouseInside = false;
	bool m_isMouseLeftDown = false;
	PointF m_dragSpot;

private:
	GameState* m_parent = nullptr;
	std::vector<Input::IReceiver*> m_receivers;
	std::vector<UIObject*> m_uiObjects;
};
