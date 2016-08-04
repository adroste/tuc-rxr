#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Framework/UI/UIButtonText.h"

class StateDev : public GameState
{
public:
	StateDev()
		:
		m_btnBack(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "A"),
		m_dlgTest()
	{
		m_btnBack.adjustToFontHeadline();
		m_btnBack.setOrigin({ 10.0f, Framework::STD_DRAW_Y - (m_btnBack.getDim().y + 10.0f) });
		m_btnBack.registerMe(this);
		m_dlgTest.setDim({ 300.0f, 300.0f });
		m_dlgTest.setCenter(Framework::getScreenCenter());
		m_dlgTest.registerMe(this);
		m_dlgTest.setZIndex(1);
	}

	virtual ~StateDev() override
	{}

	virtual bool update(float dt) override
	{
		if (m_btnBack.isClicked(true))
			//setNextState(TransitionState::Discard);
			m_dlgTest.show();

		if (m_dlgTest.getResult() == UIDialog::Result::Cancel)
		{
			m_dlgTest.show();
			m_dlgTest.setDim({ 300.0f, 300.0f });
			m_dlgTest.setCenter(Framework::getScreenCenter());
		}

		return false;
	}

	virtual void composeFrame(Drawing& draw, float dt) override
	{
		m_btnBack.draw(draw);
		m_dlgTest.draw(draw);
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
	UIDialog m_dlgTest;
};
