#pragma once
#include "UIContainer.h"
#include "../../Utility/Mutex.h"

/*
	This Container can order Items like the following:

		ItemA	ItemB
		Test	Test2
		Caption
		Left	Right

	How to use
		1. add all items to lister
		2. call registerMe
		3. call orderItems (metrics of all objects MUST be valid)
*/
class UIItemLister : public UIContainer
{
	struct Item
	{
		UIObject* left = nullptr;
		UIObject* right = nullptr;
	};
public:
	UIItemLister()
		:
		UIContainer(true)
	{}
	virtual ~UIItemLister()
	{}
	// arguments may be nullptr
	void addToList(UIObject* left, UIObject* right)
	{
		Item i;
		i.left = left;
		i.right = right;

		LockGuard g(m_muItms);
		m_items.push_back(i);
		g.unlock();

		if (left)
			left->registerMe(this);
		if (right)
			right->registerMe(this);
	}
	void orderItems()
	{
		float curY = m_pos.y + PADDING;

		// TODO adjustable width
		float widthLeft = m_dim.x / 2.0f;
		float widthRight = widthLeft;

		float leftStart = m_pos.x + PADDING;
		float leftEnd = m_pos.x + widthLeft - PADDING;
		float rightStart = m_pos.x + widthLeft + PADDING;
		float rightEnd = m_pos.x + widthLeft + widthRight - PADDING;

		for(auto& i : m_items)
		{
			float maxHei = 0.0f;
			if (i.left)
				maxHei = i.left->getDim().y;
			if (i.right)
				maxHei = std::max(maxHei, i.right->getDim().y);

			if (i.left)
				clipToRect(i.left, RectF(leftStart, curY, leftEnd, curY + maxHei));

			if (i.right)
				clipToRect(i.right, RectF(rightStart, curY, rightEnd, curY + maxHei));

			curY += maxHei;
		}

		sortReceivers();
	}

	virtual void draw(Drawing& draw) override
	{
		if(isVisible())
		{
			UIContainer::draw(draw);

			LockGuard g(m_muItms);
			for (auto& i : m_items)
			{
				if (i.left)
					i.left->draw(draw);

				if (i.right)
					i.right->draw(draw);
			}
		}
	}
private:
	void clipToRect(UIObject* o, const RectF& r)
	{
		float y = (r.y1 + r.y2) / 2.0f - o->getDim().y / 2.0f;
		float x = (r.x1 + r.x2) / 2.0f - o->getDim().x / 2.0f;

		o->setOrigin({ x,y });
	}

protected:
	std::vector<Item> m_items;
	Mutex m_muItms;

	const float PADDING = 5.0f;
};