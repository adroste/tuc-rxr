#include "UIObject.h"
#include "../../System/Input.h"
#include "WindowManager.h"

void UIObject::setVisibility(bool visible)
{
	m_isVisible = visible;

	if (m_isVisible)
	{
		m_isEnabled ? Input::IReceiver::enable() : Input::IReceiver::disable();
		WindowManager* wm = getWindowManager();
		if (wm)
			wm->onWindowShow(this);
	}
	else
	{
		Input::IReceiver::disable();
	}
}

void UIObject::setWindowDesc(std::unique_ptr<WindowDesc> wd)
{
	m_pWindowDesc = std::move(wd);
}

std::unique_ptr<WindowDesc>& UIObject::getWindowDesc()
{
	return m_pWindowDesc;
}

WindowManager* UIObject::getWindowManager() const
{
	if (!m_pWindowDesc)
		return nullptr;
	return m_pWindowDesc->getWindowManager();
}
