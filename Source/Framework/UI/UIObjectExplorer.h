#pragma once
#include "UIContainer.h"
#include "UIObjectList.h"

/*
	Imagine you have some cool pictures and want to browse them in a container like this:

	_________________________
	|						|
	| Pic1	Pic2	Pic3	|
	|						|
	| Big	Pic4	Pic5	|
	| Pic					|
	|						|
	| Pic6	Pic7			|
	|						|
	-------------------------

	just like the windows explorer

	Usage:
	
	1. set Dim for ObjectExplorer
	2. add Items (dims must be valid)

	call registerMe and setOrigin for the container
*/

class UIObjectExplorer : public UIContainer
{
public:
	UIObjectExplorer(bool show)
		:
		UIContainer(show),
		m_curIns(PointF(PADD)),
		m_curMaxY(PADD)
	{
	}

	void addObject(std::unique_ptr<UIObject> o)
	{
		assert(o);

		// search position
		if (m_curIns.x + o->getDim().x > getDim().x - PADD)
		{
			// use next row
			m_curIns = PointF(PADD, m_curMaxY + PADD);
		}
		o->setOrigin(m_curIns);
		m_curIns.x += PADD + o->getDim().x;
		m_curMaxY = std::max(m_curMaxY, o->getDim().y + o->getOrigin().y);

		m_objList.addAndReg(o.get(), this);
		LockGuard g(m_muObj);
		m_objs.push_back(std::move(o));
	}

	void clear()
	{
		LockGuard g(m_muObj);
		m_objs.clear();
		m_objList.clear();
		m_curIns = PointF(PADD);
		m_curMaxY = PADD;
	}

	void erase(UIObject* obj)
	{
		auto it = m_objs.begin();
		while (it != m_objs.end() && it->get() != obj)
			++it;

		if (it != m_objs.end())
		{
			LockGuard g(m_muObj);
			m_objList.remove(obj);
			m_objs.erase(it);
			reorder();
		}
	}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;
		UIContainer::draw(draw);
		pushDrawTransforms(draw);
		LockGuard g(m_muObj);
		for (auto& o : m_objs)
			o->draw(draw);
		g.unlock();
		popDrawTransforms(draw);
	}

	size_t size() const
	{
		return m_objs.size();
	}
private:
	void reorder()
	{
		m_curIns = PointF(PADD);
		m_curMaxY = PADD;
		for (auto& o : m_objs)
		{
			if (m_curIns.x + o->getDim().x > getDim().x - PADD)
			{
				// use next row
				m_curIns = PointF(PADD, m_curMaxY + PADD);
			}
			if (o->getOrigin() != m_curIns)
				o->setOrigin(m_curIns);

			m_curIns.x += PADD + o->getDim().x;
			m_curMaxY = std::max(m_curMaxY, o->getDim().y + o->getOrigin().y);
		}
	}


private:
	const float PADD = 10.0f;
	std::vector<std::unique_ptr<UIObject>> m_objs;
	UIObjectList m_objList;
	Mutex m_muObj;
	PointF m_curIns = PointF(10.0f); // current insert position
	float m_curMaxY = 10.0f; // max y of all objects
	// iterateable
public:
	decltype(m_objs)::iterator begin()
	{
		return m_objs.begin();
	}

	decltype(m_objs)::iterator end()
	{
		return m_objs.end();
	}

	decltype(m_objs)::const_iterator begin() const
	{
		return m_objs.cbegin();
	}

	decltype(m_objs)::const_iterator end() const
	{
		return m_objs.cend();
	}
};
