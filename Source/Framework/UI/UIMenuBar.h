#pragma once
#include "UIContainer.h"
#include <functional>

/*
	Menu Layout:

	Section1	|	Section2	|	Section3
	Item1			Item1			Item1
	Item2			Item2
					Item3
	
	// Usage:
	1. add Sections
	2. add Items for the Sections
	3. call registerMe
	4. call order items / after every resize
	-- dont set metrics or origin!
*/

class UIMenuBar : public UIContainer
{
	using callFunc = std::function<void(const std::string&)>;

	class Section final : public UIButton
	{
		class Item final : public UIButton
		{
		public:
			Item(const std::string& name, callFunc onPress, Font& font, Section& parent)
				:
				UIButton(Style::Royal),
				m_font(font),
				m_name(name),
				m_onPress(onPress)
			{
				IMetrics::setDim(font.getDim(name));
				setOnClickCallback([this,&parent](IClickable*)
				{
					m_onPress(m_name);
					parent.close();
				});
				IReceiver::setZIndex(2);
			}

			virtual void draw(Drawing& draw) override
			{
				draw.rect(getRect(), Color::Gray());
				m_font.write(draw, m_name, getOrigin());
			}
		private:
			Font& m_font;
			const std::string m_name;
			const std::function<void(const std::string&)> m_onPress;
		};

	public:
		Section(const std::string& name, Font& font)
			:
			UIButton(Style::Royal),
			m_font(font),
			m_name(name)
		{
			IMetrics::setDim(font.getDim(name));
			setOnClickCallback([this](IClickable*)
			{
				open();
			});
			IReceiver::setZIndex(1);
		}

		virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
		{
			close();
			bool handled = UIButton::mouseDown(mpos, button);
			return handled;
		}
		void open()
		{
			if (m_open)
				return;

			m_open = true;
			for (auto& i : m_itms)
				i->enable();
		}
		void close()
		{
			if (!m_open)
				return;

			m_open = false;
			for (auto& i : m_itms)
				i->disable();
		}
		virtual void draw(Drawing& draw) override
		{
			m_font.setColor(Color::White());
			m_font.write(draw, m_name, getOrigin());
			
			if(m_open)
				for (auto& o : m_itms)
					o->draw(draw);
		}

		void addItem(const std::string& name, callFunc func, UIObjectList& olist)
		{
			m_itms.push_back(std::unique_ptr<Item>(new Item(name, func, m_font,*this)));
			olist.add(m_itms.back().get());
			m_itms.back()->disable();
		}

		const std::string& getName() const
		{
			return m_name;
		}

		virtual void setOrigin(const PointF& p) override
		{
			UIButton::setOrigin(p);

			// set origin + metrics of items
			float maxWi = 0.0f;
			for (const auto& o : m_itms)
				maxWi = std::max(maxWi, o->getDim().x);

			float curY = getOrigin().y + getDim().y;
			for(const auto& o : m_itms)
			{
				o->setWidth(maxWi);
				o->setOrigin({ m_pos.x,curY });
				curY += o->getDim().y;
			}
		}
	private:
		Font& m_font;
		const std::string m_name;
		std::vector<std::unique_ptr<Item>> m_itms;
		bool m_open = false;
	};

public:
	UIMenuBar(Font& font)
		:
		UIContainer(true),
		m_font(font)
	{
	}

	virtual void draw(Drawing& draw) override
	{
		draw.rect(getRect(), Color::Gray());
		draw.getTransform().pushModel(m_matTransform);
		draw.pushIgnoreRect();
		{
			for (auto& s : m_secs)
				s->draw(draw);
		}
		draw.popClippingRect();
		draw.getTransform().popModel();
	}

	void addSection(const std::string& name)
	{
		m_secs.push_back(std::unique_ptr<Section>(new Section(name, m_font)));
		m_objList.add(m_secs.back().get());
	}

	void addItem(const std::string& section, const std::string& name, callFunc onPress = [](const std::string&)
	             {
	             })
	{
		for (auto& s : m_secs)
		{
			if (s->getName() == section)
			{
				s->addItem(name, onPress, m_objList);
				return;
			}
		}
		// section not available
		assert(false);
	}

	virtual void registerMe(IBroadcaster* broadcaster) override
	{
		UIContainer::registerMe(broadcaster);
		m_objList.registerAll(this);

		orderItems();
	}

	void orderItems()
	{
		setOrigin(Framework::getCamOrigin());
		setDim({ Framework::getCamDim().x,m_font.getMaxHeight() });

		// set origin for sections
		float curX = 0.0f;
		for (auto& s : m_secs)
		{
			s->setOrigin({ curX,0.0f });
			curX += s->getDim().x + 10.0f;
		}
	}
private:
	Font& m_font;
	std::vector<std::unique_ptr<Section>> m_secs;
	UIObjectList m_objList;
};
