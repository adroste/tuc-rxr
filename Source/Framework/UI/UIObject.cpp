#include "UIObject.h"
#include "../../System/Input.h"
#include "WindowManager.h"
#include "WindowLayer.h"

void UIObject::setVisibility(bool visible)
{
	m_isVisible = visible;

	if (m_isVisible)
	{
		m_isEnabled ? Input::IReceiver::enable() : Input::IReceiver::disable();
		auto wl = getWindowLayer();
		if (wl)
			wl->onWindowShow(this);
	}
	else
	{
		Input::IReceiver::disable();
	}
}

void UIObject::setOrigin(const PointF& p)
{
	IMetrics::setOrigin(p);
	auto wl = getWindowLayer();
	if (wl)
		wl->updateWindows();
}

void UIObject::setDim(const PointF& d)
{
	IMetrics::setDim(d);
	auto wl = getWindowLayer();
	if (wl)
		wl->updateWindows();
}

void UIObject::setWindowDesc(std::unique_ptr<WindowDesc> wd)
{
	m_pWindowDesc = std::move(wd);
}

std::unique_ptr<WindowDesc>& UIObject::getWindowDesc()
{
	return m_pWindowDesc;
}

WindowLayer* UIObject::getWindowLayer() const
{
	if (!m_pWindowDesc)
		return nullptr;
	return m_pWindowDesc->getWindowLayer();
}

WindowManager* UIObject::getWindowManager() const
{
	WindowLayer* wl = getWindowLayer();
	if (!wl)
		return nullptr;
	return wl->getWindowManager();
}
