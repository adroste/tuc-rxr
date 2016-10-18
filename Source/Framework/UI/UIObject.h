#pragma once
#include "../OpenGL/IDrawable.h"
#include "Interfaces/IMetrics.h"
#include "../../System/Input.h"
#include "WindowDesc.h"

class WindowManager;

class UIObject : public IDrawable, public IMetrics, public Input::IReceiver
{
public:
	UIObject()
		:
		m_isEnabled(Input::IReceiver::isEnabled())
	{}

	virtual ~UIObject() override
	{}

	// IMPORTANT: do not override input methods (IReceiver)!!

	virtual void enable() override
	{
		m_isEnabled = true;
		if (m_isVisible)
			Input::IReceiver::enable();		
	}

	virtual void disable() override
	{
		m_isEnabled = false;
		if (m_isVisible)
			Input::IReceiver::disable();
	}

	virtual void setVisibility(bool visible);

	virtual void show()
	{
		setVisibility(true);
	}

	virtual void hide()
	{
		setVisibility(false);
	}

	bool isVisible() const
	{
		return m_isVisible;
	}

	void setWindowDesc(std::unique_ptr<WindowDesc> wd);

	std::unique_ptr<WindowDesc>& getWindowDesc();

	WindowManager* getWindowManager() const;

private:
	PointF m_padding;
	bool m_isEnabled;
	bool m_isVisible = true;
	std::unique_ptr<WindowDesc> m_pWindowDesc = nullptr;
};
