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
		m_objs.sort([](const UIObject* l, const UIObject* r)
		{
			// TODO check
			return l->getZIndex() > r->getZIndex();
		});
	}

	void registerAll(Input::IBroadcaster* broadcaster)
	{
		for (auto o : m_objs)
			o->registerMe(broadcaster);
	}

	void add(UIObject* obj)
	{
		m_objs.push_back(obj);
		sort();
	}

	void remove(UIObject* obj)
	{
		m_objs.remove_if([obj](const UIObject* o)
		{
			return obj == o;
		});
	}

	virtual void draw(Drawing& draw) override
	{
		for (auto o : m_objs)
			if(o->isVisible())
				o->draw(draw);
	}

private:
	std::list<UIObject*> m_objs;
};
