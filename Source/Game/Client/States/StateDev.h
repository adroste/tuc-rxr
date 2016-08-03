#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Framework/UI/UIButtonText.h"

class StateDev : public GameState
{
public:
	StateDev()
		:
		m_btnBack(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "A")
	{
		m_btnBack.adjustToFontHeadline();
		m_btnBack.setOrigin({ 300.0f, Framework::STD_DRAW_Y - (m_btnBack.getDim().y + 10.0f) });
		m_btnBack.registerMe(this);
	}

	virtual ~StateDev() override
	{
	}

	virtual bool update(float dt) override
	{
		if (m_btnBack.isClicked(true))
			setNextState(TransitionState::Discard);

		return false;
	}
	virtual void composeFrame(Drawing& draw, float dt) override
	{
		m_btnBack.draw(draw);
	}

private:
	UIButtonText m_btnBack;
};
