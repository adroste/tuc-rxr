#pragma once
#include "UIObject.h"
#include "../../System/Input.h"
#include "../GameState.h";

class UIWindow : public UIObject, public Input::IReceiver
{
public:
	UIWindow()
	{
	}
	virtual ~UIWindow()
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

		for (auto obj : m_uiObjects)
		{
			obj->draw(draw);
		}
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
		// TODO move window
		return getRect().isPointInside(mpos);
	}

	virtual void registerMe(GameState* parent) override
	{
		m_parent = parent;
		Input::IReceiver::registerMe(parent);
	}

	virtual void unregisterMe(GameState* parent) override final
	{
		for (auto r : m_receivers)
		{
			r->unregisterMe(parent);
		}
		Input::IReceiver::unregisterMe(parent);
	}

	virtual void addUIObject(UIObject* obj)
	{
		m_uiObjects.push_back(obj);
		Input::IReceiver* pRec = dynamic_cast<Input::IReceiver*>(obj);
		if (pRec)
			m_receivers.push_back(pRec);
	}

private:
	GameState* m_parent = nullptr;
	std::vector<Input::IReceiver*> m_receivers;
	std::vector<UIObject*> m_uiObjects;
};
