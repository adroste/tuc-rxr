#pragma once
#include "UIObjectList.h"
#include "WindowDesc.h"
#include "../../System/Input.h"

class WindowLayer : public Input::IReceiver, Input::IBroadcaster
{
	friend class WindowManager;
public:
	class Anchor
	{
	public:
		enum
		{
			Center = 0,
			Left = 1,
			Top = 2,
			Right = 4,
			Bottom = 8
		};
	};

public:
	WindowLayer()
	{}

	virtual ~WindowLayer()
	{}

	void addWindow(UIObject* obj, size_t anchor = 0, PointF offset = PointF(0.0f))
	{
		LockGuard g(m_muUiWindow);

		std::unique_ptr<WindowDesc> pCd = std::unique_ptr<WindowDesc>(new WindowDesc(this, anchor, offset));
		m_uiWindows.addAndReg(obj, this);
		obj->setWindowDesc(std::move(pCd));
		updateWindowOrigin(obj);
		onWindowShow(obj);
	}

	void removeWindow(UIObject* obj)
	{
		LockGuard g(m_muUiWindow);
		m_uiWindows.remove(obj);
		obj->setWindowDesc(nullptr);
	}

	void onWindowShow(UIObject* obj)
	{
		if (!obj->isVisible()) return;

		m_uiWindows.setFocusFor(obj);
		RectF rct = obj->getRect();
		for (const auto& o : m_uiWindows)
		{
			if (o == obj || !o->isVisible()) continue;
			if (rct.isRectCutting(o->getRect()))
				o->hide();
		}
	}

	void drawWindows(Drawing& draw)
	{
		LockGuard g(m_muUiWindow);
		m_uiWindows.draw(draw);
	}

	void updateWindows()
	{
		if (m_suppressUpdate) return;
		m_suppressUpdate = true;

		for (const auto& o : m_uiWindows)
			updateWindowOrigin(o);

		m_suppressUpdate = false;
	}

	WindowManager* getWindowManager() const
	{
		return m_windowManager;
	}

private:
	void setWindowManager(WindowManager* wm)
	{
		m_windowManager = wm;
	}

	static void updateWindowOrigin(UIObject* obj)
	{
		WindowDesc* desc = obj->getWindowDesc().get();
		if (!desc) return;

		obj->setCenter(Framework::getScreenCenter());

		if (desc->m_anchor & Anchor::Left && !(desc->m_anchor & Anchor::Right))
			obj->setLeft(desc->m_offset.x);
		else if (desc->m_anchor & Anchor::Right && !(desc->m_anchor & Anchor::Left))
			obj->setRight(desc->m_offset.x);
		if (desc->m_anchor & Anchor::Top && !(desc->m_anchor & Anchor::Bottom))
			obj->setTop(desc->m_offset.y);
		else if (desc->m_anchor & Anchor::Bottom && !(desc->m_anchor & Anchor::Top))
			obj->setBottom(desc->m_offset.y);
	}

private:
	UIObjectList m_uiWindows;
	Mutex m_muUiWindow;

	WindowManager* m_windowManager = nullptr;
	bool m_suppressUpdate = false;
};
