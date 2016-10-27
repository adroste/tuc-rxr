#pragma once
#include "UIObjectList.h"
#include "WindowDesc.h"
#include "../../System/Input.h"

class WindowManager : public Input::IReceiver, public Input::IBroadcaster
{
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
	WindowManager()
	{
		Input::registerListener(this);
	}

	virtual ~WindowManager()
	{
		Input::unregisterListener(this);
	}

	void addWindow(UIObject* obj, size_t anchor = 0, PointF offset = PointF(0.0f), bool isDialog = false)
	{
		LockGuard g(m_muWiMa);

		// if already registered somewhere
		auto wm = obj->getWindowManager();
		if (wm)
			wm->removeWindow(obj);

		std::unique_ptr<WindowDesc> pCd = std::unique_ptr<WindowDesc>(new WindowDesc(this, anchor, offset, isDialog));
		m_objs.addAndReg(obj, this);
		obj->setWindowDesc(std::move(pCd));
		m_suppressUpdate = true;
		updateWindowOrigin(obj);
		m_suppressUpdate = false;
		onWindowShow(obj);
	}

	void removeWindow(UIObject* obj)
	{
		LockGuard g(m_muWiMa);
		m_objs.remove(obj);
		obj->setWindowDesc(nullptr);
	}

	void onWindowShow(UIObject* obj)
	{
		if (!obj->isVisible()) return;
		LockGuard g(m_muWiMa);
		m_suppressUpdate = true;
		// sets Focus if obj is Dialog		
		setFocusForDialog(obj);
		m_suppressUpdate = false;
		hideCuttingWindows(obj);
	}

	void hideCuttingWindows(UIObject* obj)
	{
		if (!obj->isVisible() || obj->getWindowDesc()->isDialog())
			return;

		LockGuard g(m_muWiMa);

		RectF rct = obj->getRect();
		for (const auto& o : m_objs)
		{
			if (o->getZIndex() != obj->getZIndex() || o == obj || !o->isVisible()) continue;
			if (rct.isRectCutting(o->getRect()))
				o->hide();
		}
	}

	void drawWindows(Drawing& draw)
	{
		LockGuard g(m_muWiMa);
		m_objs.draw(draw);
	}

	void updateWindows()
	{
		if (m_suppressUpdate) return;

		LockGuard g(m_muWiMa);

		m_objs.sort();

		m_suppressUpdate = true;
		for (const auto& o : m_objs)
			updateWindowOrigin(o);
		m_suppressUpdate = false;

		for (const auto& w : m_objs)
			hideCuttingWindows(w);
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
		return sendMouseMove(mpos, mdiff, handled);
	}
	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		return sendMouseDown(mpos, button);
	}
	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		return sendMouseUp(mpos, button);
	}
	virtual bool wheel(const PointF& mpos, float amount) override
	{
		return sendWheel(mpos, amount);
	}

private:
	void setFocusForDialog(UIObject* obj)
	{
		if (!obj->getWindowDesc()->isDialog()) return;

		// get highest index
		int maxZ = 0;
		for (const auto& o : m_objs)
				maxZ = std::max(maxZ, o->getZIndex());

		obj->setZIndex(maxZ + 1);
		m_objs.sort();
	}

	static void updateWindowOrigin(UIObject* obj)
	{
		WindowDesc* desc = obj->getWindowDesc().get();
		if (!desc) return;

		obj->setCenter(Framework::getScreenCenter() + desc->m_offset);

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
	UIObjectList m_objs;
	Mutex m_muWiMa;

	bool m_suppressUpdate = false;
};
