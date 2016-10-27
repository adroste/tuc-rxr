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
			PointF pos = getOrigin();
			PointF dim = getDim();
			draw.rect(RectF(pos, pos + PointF(dim.x, dim.y / 2.0f)), Color(m_cd.diffuse).fromGamma());
			draw.rect(RectF(pos + PointF(0.0f, dim.y / 2.0f), pos + dim), Color(m_cd.spec).fromGamma());
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
	CALLBACK(BucketErase, UIContainerBucket*);
public:
	UIContainerBucket(bool show)
		:
		UIObjectExplorer(show)
	{
	}

	void addMaterial(const CubeDesc& c)
	{
		auto p = owner_ptr<DescBox>(new DescBox(c));
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
					m_onBucketErase(this);
					break;
				}
			}
		}
		return h;
	}

	std::vector<CubeDesc> getCubeDesc() const
	{
		std::vector<CubeDesc> v;
		for (const auto& o : *this)
		{
			auto b = dynamic_cast<const DescBox*>(o.get());
			if (b)
			{
				v.push_back(b->getCubeDesc());
			}
		}
		return v;
	}
};
