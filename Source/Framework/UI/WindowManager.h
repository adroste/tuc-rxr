#pragma once
#include "UIObjectList.h"
#include "WindowDesc.h"
#include "../../System/Input.h"
#include "WindowLayer.h"

class WindowManager : public Input::IReceiver, Input::IBroadcaster
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
	{}

	virtual ~WindowManager()
	{}

	void addLayer(WindowLayer* layer)
	{
		LockGuard g(m_muWindowManager);

		m_layers.push_back(layer);
		layer->registerMe(this);
	}

	void addLayer(WindowLayer* layer, size_t pos)
	{
		LockGuard g(m_muWindowManager);

		if (pos >= m_layers.size())
		{
			m_layers.push_back(layer);
		}
		else
		{
			auto it = m_layers.begin();
			for (size_t i = 0; i < pos; ++i) ++it;
			m_layers.insert(it, layer);
		}

		layer->registerMe(this);
	}

	void removeLayer(WindowLayer* layer)
	{
		LockGuard g(m_muWindowManager);

		m_layers.remove_if([layer](const WindowLayer* wl)
		{
			return layer == wl;
		});
		layer->unregisterMe();
	}

	void removeLayer()
	{
		LockGuard g(m_muWindowManager);
		auto l = m_layers.back();
		m_layers.pop_back();
		l->unregisterMe();
	}

	void removeLayer(size_t pos)
	{
		if (pos >= m_layers.size())
			return;


	}

	void drawLayer(Drawing& draw)
	{
		LockGuard g(m_muUiWindow);
		m_uiWindows.draw(draw);
	}

	void updateLayer()
	{
		if (m_suppressUpdate) return;
		m_suppressUpdate = true;

		for (const auto& o : m_uiWindows)
			updateWindowOrigin(o);

		m_suppressUpdate = false;
	}

private:
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
	std::list<WindowLayer*> m_layers;
	Mutex m_muWindowManager;
};
