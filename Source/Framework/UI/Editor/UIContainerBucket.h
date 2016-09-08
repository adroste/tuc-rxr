#pragma once
#include "../UIContainer.h"
#include "../UIObjectExplorer.h"

class UIContainerBucket : public UIObjectExplorer
{
	class DescBox : public UIButton
	{
	public:
		DescBox(const CubeDesc& c)
			: UIButton(Style::Royal),
			  m_cd(c)
		{
		}

		virtual ~DescBox()
		{
		}

		virtual void draw(Drawing& draw) override
		{
			draw.rect(RectF(m_pos, m_pos + PointF(m_dim.x, m_dim.y / 2.0f)), Color(m_cd.diffuse));
			draw.rect(RectF(m_pos + PointF(0.0f, m_dim.y / 2.0f), m_pos + m_dim), Color(m_cd.spec));
		}

		const CubeDesc& getCubeDesc() const
		{
			return m_cd;
		}

		bool deleteMe() const
		{
			return m_plsDelete;
		}

		virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
		{
			bool h = UIButton::mouseDown(mpos, button);
			if (button == Input::Mouse::Right)
			{
				if (getRect().isPointInside(mpos))
				{
					m_plsDelete = true;
				}
			}
			return h;
		}

	private:
		const CubeDesc m_cd;
		bool m_plsDelete = false;
	};

public:
	UIContainerBucket()
	{
	}

	void addMaterial(const CubeDesc& c)
	{
		auto p = std::unique_ptr<DescBox>(new DescBox(c));
		p->setDim(PointF(50.0f));
		addObject(std::move(p));
	}


	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		bool h = UIObjectExplorer::mouseDown(mpos, button);
		if(h)
		{
			// check if button wants to be deleted
			for(auto& o : *this)
			{
				auto b = dynamic_cast<DescBox*>(o.get());
				if(b && b->deleteMe())
				{
					erase(b);
					break;
				}
			}
		}
		return h;
	}
};
