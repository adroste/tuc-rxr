#pragma once
#include "UIObject.h"
#include "UIObjectList.h"

class WindowManager
{
	using etype = std::pair<UIObject*, std::pair<size_t, PointF>>;
public:
	class Anchor
	{
	public:
		enum 
		{
			Left = 1,
			Top = 2,
			Right = 4,
			Bottom = 8
		};
	};

public:
	WindowManager()
	{}

	virtual ~WindowManager()
	{}

	void addUIElement(UIObject* obj, Input::IBroadcaster* broadcaster, size_t anchor, PointF offset = PointF(0.0f))
	{
		LockGuard g(m_muUiElements);
		obj->registerMe(broadcaster);
		m_uiElements.push_back(std::make_pair(obj, std::make_pair(anchor, offset)));
		obj->setWindowManager(this);
		sortUIElements();
		updateUIElementOrigin(obj, anchor, offset);
	}

	void removeUIElement(UIObject* obj)
	{
		LockGuard g(m_muUiElements);
		m_uiElements.remove_if([obj](const etype p)
		{
			return obj == p.first;
		});
		obj->unregisterMe();
		obj->setWindowManager(nullptr);
	}

	void onUIElementShow(UIObject* obj)
	{
		if (!obj->isVisible()) return;

		RectF rct = obj->getRect();
		for (const auto& o : m_uiElements)
		{
			if (rct.isRectCutting(o.first->getRect()))
				o.first->hide();
		}
	}

	void drawUIElements(Drawing& draw)
	{
		LockGuard g(m_muUiElements);

		for (auto it = m_uiElements.rbegin(); it != m_uiElements.rend(); ++it)
			if ((*it).first->isVisible())
				(*it).first->draw(draw);
	}

	void updateUIElements()
	{
		return;
		for (const auto& e : m_uiElements)
			updateUIElementOrigin(e.first, e.second.first, e.second.second);
	}

private:
	void sortUIElements()
	{
		LockGuard g(m_muUiElements);
		m_uiElements.sort([](const etype l, const etype r)
		{
			return l.first->getZIndex() > r.first->getZIndex();
		});
	}

	static void updateUIElementOrigin(UIObject* obj, size_t anchor, PointF offset)
	{
		obj->setCenter(Framework::getScreenCenter());

		if (anchor & Anchor::Left && !(anchor & Anchor::Right))
			obj->setLeft(offset.x);
		else if (anchor & Anchor::Right && !(anchor & Anchor::Left))
			obj->setRight(offset.x);
		if (anchor & Anchor::Top && !(anchor & Anchor::Bottom))
			obj->setTop(offset.x);
		else if (anchor & Anchor::Bottom && !(anchor & Anchor::Top))
			obj->setBottom(offset.x);
	}

private:
	std::list<etype> m_uiElements;
	Mutex m_muUiElements;
};
