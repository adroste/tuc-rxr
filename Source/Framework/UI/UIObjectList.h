#pragma once
#include "UIObject.h"

class UIObjectList : public IDrawable
{
public:
	UIObjectList()
	{}
	UIObjectList(std::initializer_list<UIObject*> objs)
		:
		m_objs(objs)
	{
		sort();
	}

	virtual ~UIObjectList()
	{}

	void sort()
	{
		LockGuard g(m_muObjs);
		m_objs.sort([](const UIObject* l, const UIObject* r)
		{
			// TODO check
			return l->getZIndex() > r->getZIndex();
		});
	}

	void registerAll(Input::IBroadcaster* broadcaster)
	{
		LockGuard g(m_muObjs);
		for (auto o : m_objs)
			o->registerMe(broadcaster);
	}

	void add(UIObject* obj)
	{
		LockGuard g(m_muObjs);
		m_objs.push_back(obj);
		sort();
	}
	void addAndReg(UIObject* obj, Input::IBroadcaster* broadcaster)
	{
		LockGuard g(m_muObjs);
		obj->registerMe(broadcaster);
		m_objs.push_back(obj);
		sort();
	}

	void remove(UIObject* obj)
	{
		LockGuard g(m_muObjs);
		m_objs.remove_if([obj](const UIObject* o)
		{
			return obj == o;
		});
		obj->unregisterMe();
	}

	virtual void draw(Drawing& draw) override
	{
		LockGuard g(m_muObjs);

		for (auto it = m_objs.rbegin(); it != m_objs.rend(); ++it)
			if ((*it)->isVisible())
				(*it)->draw(draw);
	}

	void setFocusFor(UIObject* obj)
	{
		// get highest index
		int maxZ = 0;
		for (const auto& o : m_objs)
			maxZ = std::max(maxZ, o->getZIndex());

		obj->setZIndex(maxZ + 1);

		sort();
	}

private:
	std::list<UIObject*> m_objs;
	Mutex m_muObjs;
};
