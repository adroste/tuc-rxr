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
	enum class Mode
	{
		Left,
		Middle,
		Right
	};

public:
	UIItemLister()
		:
		UIContainer(true),
		m_mode(Mode::Left)
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
		float curY = /*m_pos.y + */PADDING + m_wallPadd;

		// TODO adjustable width
		float widthLeft = m_dim.x / 2.0f;
		float widthRight = widthLeft;

		float leftStart = /*m_pos.x + */PADDING;
		float leftEnd = /*m_pos.x + */widthLeft - PADDING;
		float rightStart = /*m_pos.x + */widthLeft + PADDING;
		float rightEnd = /*m_pos.x + */widthLeft + widthRight - PADDING;

		auto clipFunc = &UIItemLister::clipToRectMid;
		if (m_mode == Mode::Left)
			clipFunc = &UIItemLister::clipToRectLeft;
		else if (m_mode == Mode::Right)
			clipFunc = &UIItemLister::clipToRectRight;

		for(auto& i : m_items)
		{
			float maxHei = 0.0f;
			if (i.left)
				maxHei = i.left->getDim().y;
			if (i.right)
				maxHei = std::max(maxHei, i.right->getDim().y);

			if (i.left)
				(this->*clipFunc)(i.left, RectF(leftStart, curY, leftEnd, curY + maxHei));

			if (i.right)
				(this->*clipFunc)(i.right, RectF(rightStart, curY, rightEnd, curY + maxHei));

			curY += maxHei + m_cellPadd;
		}

		sortReceivers();
	}

	virtual void draw(Drawing& draw) override
	{
		if(isVisible())
		{
			UIContainer::draw(draw);

			pushDrawTransform(draw);

			LockGuard g(m_muItms);
			for (auto& i : m_items)
			{
				if (i.left)
					i.left->draw(draw);

				if (i.right)
					i.right->draw(draw);
			}

			popDrawTransform(draw);
		}
	}

	void UIItemLister::setMode(const Mode mode)
	{
		m_mode = mode;
	}
	void setCellPadding(float p)
	{
		m_cellPadd = p;
	}
	void setWallPadding(float p)
	{
		m_wallPadd = p;
	}
private:
	// ReSharper disable CppMemberFunctionMayBeStatic
	void clipToRectMid(UIObject* o, const RectF& r) const
	// ReSharper restore CppMemberFunctionMayBeStatic
	{
		float y = (r.y1 + r.y2) / 2.0f - o->getDim().y / 2.0f;
		float x = (r.x1 + r.x2) / 2.0f - o->getDim().x / 2.0f;

		o->setOrigin({ x,y });
	}
	void clipToRectLeft(UIObject* o, const RectF& r) const
	{
		float y = (r.y1 + r.y2) / 2.0f - o->getDim().y / 2.0f;
		float x = r.x1 + m_wallPadd;

		o->setOrigin({ x,y });
	}
	void clipToRectRight(UIObject* o, const RectF& r) const
	{
		float y = (r.y1 + r.y2) / 2.0f - o->getDim().y / 2.0f;
		float x = r.x2 - o->getDim().x - m_wallPadd;

		o->setOrigin({ x,y });
	}

protected:
	std::vector<Item> m_items;
	Mutex m_muItms;
	Mode m_mode;
	float m_cellPadd = 0.0f; // padding between objects
	float m_wallPadd = 0.0f; // padding to wall

protected:
	const float PADDING = 5.0f;
};
