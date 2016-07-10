#pragma once
#include "GameState.h"
#include "../UI/UIButton.h"

class StateMenu : public GameState
{
public:
	StateMenu()
	{
		m_myButt.setDim({ 300, 100 });
		m_myButt.center();

		m_myButt.registerMe(this);
	}
	virtual ~StateMenu()
	{}

	virtual bool update(float dt) override
	{

		return false;
	}

	virtual void composeFrame(float dt) override
	{
		Drawing d;

		m_myButt.draw(d);

		d.rect(RectF::constructFromPoint(m_mpos, 10.0f), Color::Red());
	}
	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		handled = GameState::mouseMove(mpos, handled);

		m_mpos = mpos;
		return handled;
	}
private:
	PointF m_mpos;
	UIButton m_myButt;
};
