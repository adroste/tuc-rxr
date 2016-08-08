#pragma once
#include "../OpenGL/IDrawable.h"
#include "Interfaces/IMetrics.h"

class UIObject : public IDrawable, public IMetrics, public Input::IReceiver
{
public:
	UIObject()
		:
		m_isEnabled(Input::IReceiver::isEnabled())
	{}

	virtual ~UIObject() override
	{}


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

	virtual bool isEnabled() const override
	{
		return m_isEnabled;
	}

	virtual void setVisibility(bool visible)
	{
		m_isVisible = visible;

		if (m_isVisible)
			m_isEnabled ? Input::IReceiver::enable() : Input::IReceiver::disable();
		else
			Input::IReceiver::disable();
	}

	bool isVisible() const
	{
		return m_isVisible;
	}

private:
	bool m_isEnabled;
	bool m_isVisible = true;
};