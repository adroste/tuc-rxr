#pragma once
#include "UIObject.h"
#include "../../System/Input.h"
#include "../GameState.h";

class UIWindow : public UIObject, public Input::IReceiver
{
public:
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

		// TODO draw border, background, window-buttons

		for (auto obj : m_uiObjects)
		{
			obj->draw(draw);
		}
	}

	virtual bool mouseDown(Input::Mouse button, const PointF& mpos) override
	{
		return getRect().isPointInside(mpos);;
	}

	virtual bool mouseUp(Input::Mouse button, const PointF& mpos) override
	{
		return getRect().isPointInside(mpos);;
	}

	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		// TODO move window
		return getRect().isPointInside(mpos);
	}

	virtual bool wheel(float amount, const PointF& mpos) override
	{
		return getRect().isPointInside(mpos);
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
			m_receivers.push_back(pRec);
	}

private:
	GameState* m_parent = nullptr;
	std::vector<Input::IReceiver*> m_receivers;
	std::vector<UIObject*> m_uiObjects;
};
