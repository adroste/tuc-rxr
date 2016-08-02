#pragma once
#include "../../../Framework/GameState.h"

class StateEditor : public GameState
{
public:
	StateEditor()
		:
		GameState(TransitionState::Discard, false),
		m_btnBack(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Back")
	{
		m_btnBack.adjustToFontHeadline();
		m_btnBack.setOrigin({ 10.0f, Framework::STD_DRAW_Y - (m_btnBack.getDim().y + 10.0f) });
		m_btnBack.registerMe(this);
	}

	virtual ~StateEditor() override
	{
	}

	virtual bool update(float dt) override
	{
		if (m_btnBack.isClicked(true))
			setNextState(nullptr);

		return false;
	}
	virtual void composeFrame(Drawing& draw, float dt) override
	{
		m_btnBack.draw(draw);
	}

private:
	UIButtonText m_btnBack;
};