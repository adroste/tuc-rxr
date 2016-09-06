#pragma once
#include "UIContainer.h"
#include "UIObjectList.h"

/*
	This Container takes unique ownership of other containes and lists + deletes them

	Usage:
	1. set Dim for ContainerLister
	2. addContainer -> this will call setDim with appropriate width if not fixed dim

	don't forget to call registerMe + setOrigin
*/

class UIContainerLister : public UIContainer
{
public:
	UIContainerLister()
		:
		UIContainer(true)
	{}

	virtual void setDim(const PointF& d) override
	{
		UIContainer::setDim(d);
		for (auto& i : m_cons)
			i->setWidth(d.x);
	}
	void addContainer(std::unique_ptr<UIObject> c)
	{
		if(!hasFixedDim())
			c->setWidth(m_dim.x);

		assert(c->getDim().x <= m_dim.x);
		c->setOrigin({ 0.0f,m_curY });
		m_curY += c->getDim().y;

		m_objList.addAndReg(c.get(), this);
		LockGuard g(m_muCo);
		m_cons.push_back(std::move(c));
	}
	// insert position -> 0 = start size() = end
	void insert(size_t pos, std::unique_ptr<UIObject> c)
	{
		assert(pos <= m_cons.size());
		if(pos == m_cons.size())
		{
			addContainer(std::move(c));
			return;
		}

		if (!hasFixedDim())
			c->setWidth(m_dim.x);

		assert(c->getDim().x <= m_dim.x);
		// determine y
		
		m_objList.addAndReg(c.get(), this);
		LockGuard g(m_muCo);
		m_cons.insert(m_cons.begin() + pos, std::move(c));
		reorder();
	}
	void clear()
	{
		LockGuard g(m_muCo);
		m_objList.clear();
		m_cons.clear();
		m_curY = 0.0f;
	}
	void erase(UIContainer* c)
	{
		auto it = m_cons.begin();
		for (; it != m_cons.end(); ++it)
			if (it->get() == c)
				break;

		if(it != m_cons.end())
		{
			// remove element
			LockGuard g(m_muCo);
			m_objList.remove(c);
			m_cons.erase(it);

			// reorder items
			reorder();
		}
	}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;
		UIContainer::draw(draw);

		pushDrawTransforms(draw, PointF(0.0f));
		LockGuard g(m_muCo);
		for (auto& c : m_cons)
			c->draw(draw);
		g.unlock();
		popDrawTransforms(draw);
	}

	void adjustToContent()
	{
		UIContainer::setDim({ m_dim.x, m_curY });
	}

	size_t size() const
	{
		return m_cons.size();
	}

private:
	// reorder origins
	void reorder()
	{
		m_curY = 0.0f;
		for(auto& c : m_cons)
		{
			PointF o = { 0.0f,m_curY };
			if (c->getOrigin() != o)
				c->setOrigin(o);
			m_curY += c->getDim().y;
		}
	}

protected:
	std::vector<std::unique_ptr<UIObject>> m_cons;

private:
	UIObjectList m_objList;
	Mutex m_muCo;
	float m_curY = 0.0f;

	// iterateable
public:
	decltype(m_cons)::iterator begin()
	{
		return m_cons.begin();
	}
	decltype(m_cons)::iterator end()
	{
		return m_cons.end();
	}
};