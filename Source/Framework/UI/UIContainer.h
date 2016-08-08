#pragma once
#include "UIObject.h"
#include "../../System/Input.h"
#include "../../System/Exceptions/ExceptionInvalidOperation.h"

class UIContainer : public UIObject, public Input::IReceiver
{
public:
	UIContainer(bool show = false)
	{
		show ? UIContainer::show() : UIContainer::hide();
	}

	virtual ~UIContainer() override
	{
		if (m_parent)
		{
			unregisterMe(m_parent);
			m_parent = nullptr;
		}
	}

	virtual void update(float dt)
	{}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		// TODO draw border, background
		// just testing
		{
			draw.rect(getRect(), Color::Gray());
			draw.rect(getRect().shrink(2.0f), Color::DarkGray());
		}

		draw.getTransform().pushModel(m_matTransform);
		for (auto obj : m_uiObjects)
		{
			obj->draw(draw);
		}
		draw.getTransform().popModel();
	}

	// Input
	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		return m_isMouseInside;
	}

	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		return m_isMouseInside;
	}

	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		m_isMouseInside = getRect().isPointInside(mpos);
		return m_isMouseInside;
	}

	virtual bool wheel(const PointF& mpos, float amount) override
	{
		return m_isMouseInside;
	}


	virtual void setOrigin(const PointF& p) override
	{
		IMetrics::setOrigin(p);
		m_matTransform = glm::translate(glm::vec3(p.x, p.y, 0.0f));
		for (auto r : m_receivers)
			r->setInpTransform(getInpTransform() * glm::translate(glm::vec3(-p.x, -p.y, 0.0f)));
	}

	virtual void setZIndex(int z) override
	{
		int zdiff = z - getZIndex();
		Input::IReceiver::setZIndex(z);

		for (auto r : m_receivers)
			r->setZIndex(r->getZIndex() + zdiff);
	}

	virtual void setInpTransform(glm::mat4 transform) override
	{
		Input::IReceiver::setInpTransform(transform);
		for (auto r : m_receivers)
			r->setInpTransform(transform * r->getInpTransform());
	}

	// register after all receivers have been added
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
	do not use: ui objects visible status should reveal if objects are visible INSIDE the container, not at all*/

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
	// every container object needs to be added with this function
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
		m_isShown ? enable() : disable();
	}

protected:
	glm::mat4 m_matTransform;
	bool m_isMouseInside = false;

private:
	GameState* m_parent = nullptr;
	std::vector<Input::IReceiver*> m_receivers;
	std::vector<UIObject*> m_uiObjects;

	bool m_isShown;
};
