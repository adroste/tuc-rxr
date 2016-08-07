#pragma once
#include "UIObject.h"
#include "../../System/Input.h"
#include "../GameState.h"

class UIWindow : public UIObject, public Input::IReceiver
{
public:
	UIWindow(bool show = false)
		:
		m_btnCancel(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "X")
	{
		m_btnCancel.setZIndex(getZIndex() + 1);
		m_btnCancel.adjustToFontHeadline();
		UIWindow::addUIObject(&m_btnCancel);

		show ? UIWindow::show() : UIWindow::hide();
	}

	virtual ~UIWindow() override
	{
		if (m_parent)
		{
			unregisterMe(m_parent);
			m_parent = nullptr;
		}
	}

	virtual void update(float dt)
	{
		if (m_btnCancel.isClicked(true))
			hide();
	}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		// TODO draw border, background
		// just testing
		{
			draw.rect(getRect(), Color::Gray());
			draw.rect(getRect().shrink(2.0f), Color::DarkGray());
		}

		PointF pos = getOrigin();
		glm::mat4 transform = glm::translate(glm::vec3(pos.x, pos.y, 0.0f));
		draw.getTransform().pushModel(transform);
		draw.getTransform().flush();
		for (auto obj : m_uiObjects)
		{
			obj->draw(draw);
		}
		draw.getTransform().popModel();
	}


	virtual bool keyUp(SDL_Scancode s) override
	{
		if (s == SDL_SCANCODE_ESCAPE)
		{
			hide();
			return true;
		}
		return false;
	}

	virtual bool mouseDown(Input::Mouse button, const PointF& mpos) override
	{
		if (m_isMouseInside && button == Input::Mouse::Left)
		{
			m_isMouseLeftDown = true;
			m_dragSpot = mpos;
		}

		return m_isMouseInside;
	}

	virtual bool mouseUp(Input::Mouse button, const PointF& mpos) override
	{
		if (button == Input::Mouse::Left)
			m_isMouseLeftDown = false;

		return m_isMouseInside;
	}

	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		m_isMouseInside = getRect().isPointInside(mpos);

		// drag window
		if (m_isMouseLeftDown)
		{
			setOrigin(getOrigin() + mpos - m_dragSpot);
			m_dragSpot = mpos;			
		}

		return m_isMouseInside;
	}

	virtual bool wheel(float amount, const PointF& mpos) override
	{
		return m_isMouseInside;
	}

	virtual void setZIndex(int z) override
	{
		int zdiff = z - getZIndex();
		Input::IReceiver::setZIndex(z);

		for (auto r : m_receivers)
			r->setZIndex(r->getZIndex() + zdiff);
	}

	// register after all UIObjects has been added
	virtual void registerMe(GameState* parent) override final
	{
		m_parent = parent;
		Input::IReceiver::registerMe(parent);
		for (auto r : m_receivers)
			r->registerMe(parent);
	}

	virtual void unregisterMe(GameState* parent) override final // do not remove this final!! 
	{
		for (auto r : m_receivers)
			r->unregisterMe(parent);
		Input::IReceiver::unregisterMe(parent);
	}

	virtual void enable() override
	{
		Input::IReceiver::enable();
		for (auto r : m_receivers)
			r->enable();
	}

	virtual void disable() override
	{
		for (auto r : m_receivers)
			r->disable();
		Input::IReceiver::disable();
	}

	/*virtual void setVisibility(bool visible) override
	{
		UIObject::setVisibility(visible);
		for (auto o : m_uiObjects)
			o->setVisibility(visible);
	}
	do not use: ui objects visible status should reveal if objects are visible INSIDE the window, not at all*/

	virtual void hide()
	{
		m_isShown = false;
		disable();
		setVisibility(false);
	}

	virtual void show()
	{
		m_isShown = true;
		setVisibility(true);
		enable();
	}

	bool isShown() const
	{
		return m_isShown;
	}

protected:
	// every window object needs to be added with this function
	virtual void addUIObject(UIObject* obj)
	{
		m_uiObjects.push_back(obj);
		Input::IReceiver* pRec = dynamic_cast<Input::IReceiver*>(obj);
		if (pRec)
		{
			if (pRec->getZIndex() <= getZIndex())
				throw ExceptionInvalidOperation("UIWindow::addUIObject", "z-index of object is <= z-index of window");
			m_receivers.push_back(pRec);
		}
	}

protected:
	bool m_isMouseInside = false;
	bool m_isMouseLeftDown = false;
	PointF m_dragSpot;
	
	UIButtonText m_btnCancel;

private:
	GameState* m_parent = nullptr;
	std::vector<Input::IReceiver*> m_receivers;
	std::vector<UIObject*> m_uiObjects;

	bool m_isShown;
};
