#pragma once
#include "UIDialog.h"
#include <type_traits>

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
class UIDialogContainerHolder : public UIDialog
{
	static_assert(std::is_base_of<UIContainer, T>::value, "type must derive from UIContainer");

public:
	UIDialogContainerHolder(std::unique_ptr<T> con, Buttons btn, bool show)
		:
		UIDialog(btn, show),
		m_pCon(std::move(con))
	{
		assert(m_pCon.get());
		m_pCon->setZIndex(1);
		m_pCon->registerMe(this);
	}
	UIDialogContainerHolder(Buttons btns, bool show)
		:
		UIDialogContainerHolder(std::unique_ptr<T>(new T(true)), btns, show)
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
		// check if client area is bigger than requested (because of dialog buttons)
		if(!m_pCon->hasFixedDim())
		{
			auto d = getClientArea().getDim();
			if(d != m_pCon->getDim())
			{
				m_pCon->setDim(d);
			}
		}
	}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		UIDialog::draw(draw);
		pushDrawTransforms(draw, PointF(0.0f));
		m_pCon->draw(draw);
		popDrawTransforms(draw);
	}
private:
	std::unique_ptr<T> m_pCon;
};