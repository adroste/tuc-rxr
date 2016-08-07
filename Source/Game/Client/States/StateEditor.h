#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Framework/UI/UIButtonText.h"
#include "StateDev.h"

class StateEditor : public GameState
{
public:
	StateEditor()
		:
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
			setNextState(TransitionState::Discard);

		return true;
	}
	virtual void composeFrame(Drawing& draw, float dt) override
	{
		m_btnBack.draw(draw);
	}

	// Input handling
	virtual bool keyDown(SDL_Scancode s) override
	{
		GameState::keyDown(s);
		return true;
	}

	virtual bool keyUp(SDL_Scancode s) override
	{
		GameState::keyUp(s);
		return true;
	}

	virtual bool charDown(char c) override
	{
		GameState::charDown(c);
		return true;
	}

	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		GameState::mouseMove(mpos, handled);
		return true;
	}

	virtual bool mouseDown(Input::Mouse button, const PointF& mpos) override
	{
		GameState::mouseDown(button, mpos);
		return true;
	}

	virtual bool mouseUp(Input::Mouse button, const PointF& mpos) override
	{
		GameState::mouseUp(button, mpos);
		return true;
	}

	virtual bool wheel(float amount, const PointF& mpos) override
	{
		GameState::wheel(amount, mpos);
		return true;
	}

private:
	UIButtonText m_btnBack;
};
