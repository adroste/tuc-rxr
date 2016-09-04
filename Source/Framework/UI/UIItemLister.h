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
		{
			left->registerMe(this);
			m_uiList.add(left);
		}
		if (right)
		{
			right->registerMe(this);
			m_uiList.add(right);
		}

	}
	void orderItems()
	{
		float curY = PADDING + m_wallPadd;

		float widthLeft = m_dim.x * m_midDivider;
		float widthRight = widthLeft;

		float leftStart = PADDING;
		float leftEnd = widthLeft - PADDING;
		float rightStart = widthLeft + PADDING;
		float rightEnd = widthLeft + widthRight - PADDING;

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
	// setting dim
	void adjustToItems()
	{
		float leftMaxX = 0.0f;
		float rightMaxX = 0.0f;

		float curY = PADDING + m_wallPadd;

		for(const auto& i : m_items)
		{
			float maxHei = 0.0f;
			if (i.left)
			{
				maxHei = i.left->getDim().y;
				leftMaxX = std::max(i.left->getDim().x, leftMaxX);
			}
			if (i.right)
			{
				maxHei = std::max(maxHei, i.right->getDim().y);
				rightMaxX = std::max(i.right->getDim().x, rightMaxX);
			}
			curY += maxHei + m_cellPadd;
		}

		// final height of all items
		curY += PADDING + m_wallPadd;
		setDim({PADDING + m_wallPadd + leftMaxX + m_cellPadd + rightMaxX + m_wallPadd + PADDING, curY});
		
		if (leftMaxX > 0.0f)
			setMidDivider(leftMaxX / (leftMaxX + rightMaxX));
		else
			setMidDivider(0.0f);
	}
	virtual void draw(Drawing& draw) override
	{
		if(isVisible())
		{
			UIContainer::draw(draw);

			pushDrawTransforms(draw);

			m_uiList.draw(draw);

			popDrawTransforms(draw);
		}
	}

	virtual void setDim(const PointF& d) override
	{
		UIContainer::setDim(d);
		orderItems();
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
	void setMidDivider(float d)
	{
		m_midDivider = d;
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
	const float PADDING = 5.0f;
	// TODO it is called a padding when its inside the content-box, otherwise it is called margin
	float m_midDivider = 0.5f; // divided in middle

	UIObjectList m_uiList;
};
