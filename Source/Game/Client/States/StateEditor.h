#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Framework/UI/UIButtonText.h"
#include "StateDev.h"

class StateEditor : public GameState
{
public:
	StateEditor()
		:
		m_btnBack(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Back"),
		m_btnDev(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Dev")
	{
		m_btnBack.adjustToFontHeadline();
		m_btnBack.setOrigin({ 10.0f, Framework::STD_DRAW_Y - (m_btnBack.getDim().y + 10.0f) });
		m_btnBack.registerMe(this);
		m_btnDev.adjustToFontHeadline();
		m_btnDev.setOrigin({ Framework::STD_DRAW_X - (m_btnDev.getDim().x + 10.0f), Framework::STD_DRAW_Y - (m_btnDev.getDim().y + 10.0f) });
		m_btnDev.registerMe(this);
	}

	virtual ~StateEditor() override
	{
	}

	virtual bool update(float dt) override
	{
		if (m_btnBack.isClicked(true))
			setNextState(TransitionState::Discard);
		if (m_btnDev.isClicked(true))
			setNextState(TransitionState::PreserveOrDiscardWithPrevious, std::unique_ptr<GameState>(new StateDev()));

		return false;
	}
	virtual void composeFrame(Drawing& draw, float dt) override
	{
		m_btnBack.draw(draw);
		m_btnDev.draw(draw);
	}

private:
	UIButtonText m_btnBack;
	UIButtonText m_btnDev;
};
