#pragma once
#include "UIObjectList.h"
#include "WindowDesc.h"
#include "../../System/Input.h"
#include "WindowLayer.h"

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

	WindowLayer* getLayerTop()
	{
		return m_layers.back();
	}

	WindowLayer* getLayerBottom()
	{
		return m_layers.front();
	}

	WindowLayer* getLayer(size_t pos)
	{
		if (pos >= m_layers.size())
			return nullptr;
		auto it = m_layers.begin();
		for (size_t i = 0; i < pos; ++i) ++it;
		return *it;
	}

	size_t getLayerPosition(WindowLayer* layer)
	{
		assert(layer);
		if (!layer)
			return SIZE_MAX; // TODO throw nullptrexception
		auto it = m_layers.begin();
		for (size_t i = 0; i < m_layers.size(); ++i)
		{
			if (*it == layer)
				return i;
		}
		// no layer found
		assert(false); // TODO throw indexoutofrangexception
		return SIZE_MAX;
	}

	void addLayer(WindowLayer* layer)
	{
		LockGuard g(m_muWindowManager);

		m_layers.push_back(layer);
		layer->registerMe(this);
		updateZIndices();
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
		updateZIndices();
	}

	void removeLayer(WindowLayer* layer)
	{
		// TODO delete layers ?
		
		LockGuard g(m_muWindowManager);

		m_layers.remove_if([layer](const WindowLayer* wl)
		{
			return layer == wl;
		});
		layer->unregisterMe();
		updateZIndices();
	}

	void removeLayer()
	{
		LockGuard g(m_muWindowManager);
		auto l = m_layers.back();
		m_layers.pop_back();
		l->unregisterMe();
		updateZIndices();
	}

	void removeLayer(size_t pos)
	{
		LockGuard g(m_muWindowManager);

		if (pos >= m_layers.size())
			return;
		auto it = m_layers.begin();
		for (size_t i = 0; i < pos; ++i) ++it;
		removeLayer(*it);
	}

	void drawLayer(Drawing& draw)
	{
		LockGuard g(m_muWindowManager);
		for (auto& l : m_layers)
			l->drawWindows(draw);
	}

	void updateLayer()
	{
		LockGuard g(m_muWindowManager);
		for (const auto& l : m_layers)
			l->updateWindows();
	}


	// Input
	virtual bool keyDown(SDL_Scancode s) override
	{
		sendKeyDown(s);
		return true;
	}
	virtual bool keyUp(SDL_Scancode s) override
	{
		sendKeyUp(s);
		return true;
	}
	virtual bool charDown(char c) override
	{
		sendCharDown(c);
		return true;
	}
	virtual bool mouseMove(const PointF& mpos, const PointF& mdiff, bool handled) override
	{
		sendMouseMove(mpos, mdiff, handled);
		return true;
	}
	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		sendMouseDown(mpos, button);
		return true;
	}
	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		sendMouseUp(mpos, button);
		return true;
	}
	virtual bool wheel(const PointF& mpos, float amount) override
	{
		sendWheel(mpos, amount);
		return true;
	}

private:
	void updateZIndices()
	{
		auto it = m_layers.begin();
		for (size_t i = 1; i <= m_layers.size(); ++i)
		{
			(*it)->setZIndex(i);
			++it;
		}
	}

private:
	std::list<WindowLayer*> m_layers;
	Mutex m_muWindowManager;
};
