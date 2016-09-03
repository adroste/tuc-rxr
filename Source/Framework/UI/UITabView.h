#pragma once
#include "UIContainer.h"
#include "UIObjectList.h"
#include "UIButtonText.h"

class UIButtonText;

class UITabView : public UIContainer
{
	enum class ControlPosition
	{
		Top,
		Left
	};

public:
	UITabView(UITabView::ControlPosition ctrlPos = ControlPosition::Top)
		:
		m_ctrlPos(ctrlPos)
	{}

	virtual ~UITabView() override
	{}


	virtual void draw(Drawing& draw) override
	{
		UIContainer::draw(draw);

		pushDrawTransforms(draw);
		// draw orders does not matter: only one container is shown & ctrls are not overlapping
		for (auto c : m_containers)
			c->draw(draw);
		for (auto& c : m_tabCtrls)
			c->draw(draw);
		popDrawTransforms(draw);
	}


	virtual void addTab(UIContainer* container, const std::string& name)
	{
		assert(container);

		auto pbtn = std::unique_ptr<UIButtonText>(new UIButtonText(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), name));

		pbtn->adjustToFontHeadline();
		m_maxCtrlWidth = std::max(pbtn->getDim().x, m_maxCtrlWidth);
		m_maxCtrlHeight = std::max(pbtn->getDim().y, m_maxCtrlHeight);
		pbtn->registerMe(this);
		pbtn->setOnClickCallback([this, container](IClickable*)
		{
			activateContainer(container);
		});

		m_tabCtrls.push_back(std::move(pbtn));

		m_containers.push_back(container);
		container->registerMe(this);

		updateMetrics();
	}

	virtual void activateContainer(UIContainer* container)
	{
		assert(m_tabCtrls.size() == m_containers.size());

		for (auto c : m_containers)
		{
			if (c == container)
			{
				//updateMetrics();
				c->show();
			}
			else
			{
				c->hide();
			}
		}
	}

	virtual void updateMetrics()
	{
		// true if top, false if left
		bool isTop = m_ctrlPos == ControlPosition::Top;

		// set controls
		PointF origin = PointF(MARGIN);
		for (auto& c : m_tabCtrls)
		{
			c->setDim({ m_maxCtrlWidth, m_maxCtrlHeight });
			c->setOrigin(origin);
			
			if (isTop)
				origin.x += m_maxCtrlWidth + MARGIN;
			else
				origin.y += m_maxCtrlHeight + MARGIN;
		}

		// set containers
		origin = PointF(0.0f);
		if (isTop)
			origin.y = m_maxCtrlHeight + 2 * MARGIN;
		else
			origin.x = m_maxCtrlWidth + 2 * MARGIN;

		PointF dim = getDim() - origin;

		for (auto c : m_containers)
		{
			c->setOrigin(origin);
			c->setDim(dim);
		}
	}

	virtual void setDim(const PointF& d) override
	{
		UIContainer::setDim(d);
		updateMetrics();
	}

private:
	ControlPosition m_ctrlPos;

	float m_maxCtrlHeight = 0.0f;
	float m_maxCtrlWidth = 0.0f;
	const float MARGIN = 5.0f;

	std::list<std::unique_ptr<UIButtonText>> m_tabCtrls;
	std::list<UIContainer*> m_containers;
};
