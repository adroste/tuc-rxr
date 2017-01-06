#pragma once
#include "UIObject.h"
#include "../../Utility/owner_ptr.h"

class UIObjectList : public IDrawable
{
public:
	UIObjectList()
	{}

	virtual ~UIObjectList()
	{}

	void sort()
	{
		LockGuard g(m_muObjs);
		m_objs.sort([](const ref_ptr<UIObject> l, const ref_ptr<UIObject> r)
		{
			return l->getZIndex() > r->getZIndex();
		});
	}

	void registerAll(Input::IBroadcaster* broadcaster)
	{
		// TODO set callback for delete
		LockGuard g(m_muObjs);
		for (auto o : m_objs)
			o->registerMe(broadcaster);
	}

    void add(void) {}
    template<typename... TObjs>
	void add(ref_ptr<UIObject> first, TObjs... args)
	{
        LockGuard g(m_muObjs);
        first.setAbandonCallback([this](ref_ptr<UIObject>& o)
                               {
                                   remove(o);
                               });
        m_objs.push_back(first);
        sort();

		add(args...);
	}

	void addAndReg(ref_ptr<UIObject> obj, Input::IBroadcaster* broadcaster)
	{
		LockGuard g(m_muObjs);
		obj->registerMe(broadcaster);
		add(obj);
	}

	void clear()
	{
		for (auto& o : m_objs)
			o->unregisterMe();
		m_objs.clear();
	}



	virtual void draw(Drawing& draw) override
	{
		LockGuard g(m_muObjs);

		for (auto it = m_objs.rbegin(); it != m_objs.rend(); ++it)
			if ((*it)->isVisible())
				(*it)->draw(draw);
	}


private:
	void remove(ref_ptr<UIObject> obj)
	{
		LockGuard g(m_muObjs);
		m_objs.remove_if([obj](const ref_ptr<UIObject>& o)
		{
			return obj == o;
		});
		obj->unregisterMe();
	}

private:
	std::list<ref_ptr<UIObject>> m_objs;
	Mutex m_muObjs;

public:
    std::list<ref_ptr<UIObject>>::const_iterator begin() const
	{
		return m_objs.cbegin();
	}

    std::list<ref_ptr<UIObject>>::const_iterator end() const
	{
		return m_objs.cend();
	}

    std::list<ref_ptr<UIObject>>::iterator begin()
	{
		return m_objs.begin();
	}

    std::list<ref_ptr<UIObject>>::iterator end()
	{
		return m_objs.end();
	}
};
