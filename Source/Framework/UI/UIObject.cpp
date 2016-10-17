#include "UIObject.h"
#include "../../System/Input.h"
#include "WindowManager.h"

void UIObject::setVisibility(bool visible)
{
	m_isVisible = visible;

	if (m_isVisible)
	{
		m_isEnabled ? Input::IReceiver::enable() : Input::IReceiver::disable();
		if (m_windowManager)
			m_windowManager->onUIElementShow(this);
	}
	else
	{
		Input::IReceiver::disable();
	}
}
