#pragma once
#include "../UIWindow.h"
#include "../UIContainerLister.h"
#include "UIContainerBucket.h"
#include "../../../Game/Client/BucketLoader.h"
#include "../UIDialogInput.h"

class UIWindowBuckets : public UIWindow
{
	CALLBACK(BucketChange, UIWindowBuckets*);

	class UIAddButton : public UIButton
	{
	public:
		UIAddButton(UIWindowBuckets& parent)
			: UIButton(Style::Royal)
		{
			setOnClickCallback([&parent](IClickable*)
				{
					parent.addBucket();
				});
		}

		virtual void setDim(const PointF& d) override
		{
			UIButton::setDim({d.x,d.x});
		}

		virtual void draw(Drawing& draw) override
		{
			draw.rect(getRect(), Color::Black());
			PointF dim = getDim();
			PointF dx = PointF(dim.x / 3.0f, 0.0f);
			PointF dy = PointF(0.0f, dim.y / 3.0f);
			draw.line(getMidpoint() + dx, getMidpoint() - dx, 3.0f, Color::White());
			draw.line(getMidpoint() + dy, getMidpoint() - dy, 3.0f, Color::White());
		}
	};

	class UIBuckPreview : public UIButton
	{
	public:
		UIBuckPreview(UIWindowBuckets& parent, UIContainerBucket& buck, size_t id)
			: UIButton(Style::Royal),
			  m_parent(parent),
			  m_bucket(buck),
			  m_id(id)
		{
			setOnClickCallback([this](IClickable*)
				{
					m_parent.selectBucket(m_id);
				});
		}

		virtual void setDim(const PointF& d) override
		{
			UIButton::setDim({d.x,d.x});
		}

		virtual void draw(Drawing& draw) override
		{
			// TODO draw bucket preview
			draw.rect(getRect(), isSelected() ? Color::Red() : Color::Green());
		}

		virtual void select() override
		{
			ISelectable::select();
			m_bucket.setVisibility(true);
			m_bucket.enable();
		}

		virtual void deselect() override
		{
			ISelectable::deselect();
			m_bucket.setVisibility(false);
			m_bucket.disable();
		}

		size_t getID() const
		{
			return m_id;
		}

	private:
		UIWindowBuckets& m_parent;
		UIContainerBucket& m_bucket;
		const size_t m_id;
	};

public:
	UIWindowBuckets(bool show, WindowManager& wm, size_t anchor = 0, PointF offset = PointF(0.0f))
		:
		UIWindow(show, wm, anchor, offset),
		m_listBucketPrev(true),
		m_btnSave(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "save"),
		m_dlgInput(false, wm)
	{
		m_btnSave.adjustToFontHeadline();
		UIWindowBuckets::setDim({300,400});

		// add the add button
		m_listBucketPrev.addContainer(owner_ptr<UIObject>(new UIAddButton(*this)));

		addBucket();

		m_listBucketPrev.registerMe(this);
		m_btnSave.registerMe(this);
		m_dlgInput.setZIndex(1);
		//m_dlgInput.registerMe(this);

		m_btnSave.setOnClickCallback([this](IClickable*)
			{
				// test if anything is selected
				for (const auto& b : m_bucks)
				{
					if(b->isVisible())
					{
						// the selected one
						m_dlgInput.setTitle("bucket name:");
						m_dlgInput.show();

						auto cubevec = b->getCubeDesc();
						if(cubevec.size())
						{
							m_dlgInput.setOnResultCallback([this, cubevec](UIDialog*)
							{
								if (m_dlgInput.getResult() == UIDialog::Result::OK)
								{
									BucketLoader::saveBucket("saves/editor/buckets/" + m_dlgInput.getInput() + ".xml", cubevec);
								}
							});
						}
						break;
					}
				}
			});
	}

	virtual void setDim(const PointF& d) override
	{
		UIWindow::setDim(d);
		const auto cl = getClientArea();

		m_listBucketPrev.setDim({50.0f,cl.getHeight()});
		m_listBucketPrev.setOrigin(cl.getTopRight() - m_listBucketPrev.getDim().zeroY());

		// place in lower left
		m_btnSave.setOrigin(getRect().getBottomLeft() - PointF(0.0f, m_btnSave.getDim().y + 10.0f));
	}

	virtual void draw(Drawing& draw) override
	{
		UIWindow::draw(draw);
		pushDrawTransforms(draw);
		
		m_listBucketPrev.draw(draw);
		m_btnSave.draw(draw);
		LockGuard g(m_muCon);
		for (auto& c : m_bucks)
		{
			if (c->isVisible())
				c->draw(draw);
		}
		g.unlock();
		m_dlgInput.draw(draw);

		popDrawTransforms(draw);
	}

	void addBucket()
	{
		// add bucket + new tab	
		auto pBuck = std::unique_ptr<UIContainerBucket>(new UIContainerBucket(true));
		const auto cl = getClientArea();
		pBuck->setOrigin(cl.getTopLeft());
		pBuck->setDim({cl.getWidth() - 50.0f, cl.getHeight() - m_btnSave.getDim().y - 20.0f});

		assert(m_listBucketPrev.size());
		m_listBucketPrev.insert(m_listBucketPrev.size() - 1, owner_ptr<UIObject>(new UIBuckPreview(*this, *pBuck, m_curID++)));

		LockGuard g(m_muCon);
		pBuck->registerMe(this);
		pBuck->setOnBucketEraseCallback([this](UIContainerBucket*)
			{
				m_onBucketChange(this);
			});
		m_bucks.push_back(std::move(pBuck));

		selectBucket(m_curID - 1);

		m_onBucketChange(this);
	}

	void selectBucket(size_t id)
	{
		// select preview button
		for (auto& b : m_listBucketPrev)
		{
			UIBuckPreview* p = dynamic_cast<UIBuckPreview*>(b.get());
			if (p)
			{
				if (p->getID() == id)
					p->select();
				else
					p->deselect();
			}
		}

		m_onBucketChange(this);
	}

	// adds material to current bucket
	void addToBucket(const CubeDesc& c)
	{
		for (auto& b : m_bucks)
		{
			if (b->isVisible())
			{
				// this is the current bucket
				b->addMaterial(c);
				break;
			}
		}

		m_onBucketChange(this);
	}

	std::vector<CubeDesc> getCurDesc() const
	{
		std::vector<CubeDesc> v;
		for (const auto& b : m_bucks)
		{
			if (b->isVisible())
			{
				// this is the current bucket
				v = b->getCubeDesc();
				break;
			}
		}

		return v;
	}

private:
	UIContainerLister m_listBucketPrev; // list of Bucket previews
	std::vector<std::unique_ptr<UIContainerBucket>> m_bucks; // actual bucket container
	UIButtonText m_btnSave;
	Mutex m_muCon;
	size_t m_curID = 0; // ids to identify buckets

	UIDialogInput m_dlgInput;
};

