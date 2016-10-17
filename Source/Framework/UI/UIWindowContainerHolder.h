#pragma once
#include <type_traits>
#include "UIWindow.h"

/*
	Usage:
	1. pass unique pointer to container
	2. set dim of passed container
	3. call adjustToContainer
	4. registerMe + setOrigin
	
	Info:
	you can acces the container via wnd.get().method() or wnd->method()
*/
template <class T>
class UIWindowContainerHolder : public UIWindow 
{
	static_assert(std::is_base_of<UIContainer, T>::value, "type must derive from UIContainer");

public:
	UIWindowContainerHolder(std::unique_ptr<T> con, bool show)
		:
		UIWindow(show),
		m_pCon(std::move(con))
	{
		assert(m_pCon.get());
		m_pCon->setZIndex(1);
		m_pCon->registerMe(this);
	}
	UIWindowContainerHolder(bool show)
		:
		UIWindowContainerHolder(std::unique_ptr<T>(new T(true)),show)
	{}

	T& get()
	{
		return *m_pCon;
	}
	const T& get() const
	{
		return *m_pCon;
	}
	T* operator->()
	{
		return m_pCon.get();
	}
	const T* operator->() const
	{
		return m_pCon.get();
	}

	void adjustToContainer()
	{
		auto o = setClientArea(m_pCon->getDim());
		m_pCon->setOrigin(o);
	}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;
		UIWindow::draw(draw);
		pushDrawTransforms(draw,PointF(0.0f));
		m_pCon->draw(draw);
		popDrawTransforms(draw);
	}
private:
	std::unique_ptr<T> m_pCon;
};