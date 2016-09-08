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

*/

class UIObjectExplorer : public UIContainer
{
public:
	UIObjectExplorer()
		:
		UIContainer(true),
		m_curIns(PADD),
		m_curMaxY(PADD)
	{}

	void addObject(std::unique_ptr<UIObject> o)
	{
		assert(o.get);

		// search position
		if(m_curIns.x + o->getDim().x > m_dim.x - PADD)
		{
			// use next row
			m_curIns = PointF(PADD, m_curMaxY + PADD);
		}
		

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
private:
	std::vector<std::unique_ptr<UIObject>> m_objs;
	UIObjectList m_objList;
	Mutex m_muObj;
	PointF m_curIns = PointF(10.0f); // current insert position
	float m_curMaxY = 10.0f; // max y of all objects

	const float PADD = 10.0f;
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
};