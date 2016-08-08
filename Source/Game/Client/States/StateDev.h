#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Framework/UI/UIButtonText.h"

class StateDev : public GameState
{
public:
	StateDev()
		:
		m_btnBack(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Back"),
		m_btnDlg(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Dialog"),
		m_btnMsgBox(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "MessageBox"),
		m_dlgTest()
	{
		m_btnBack.adjustToFontHeadline();
		m_btnBack.setOrigin({ 10.0f, Framework::STD_DRAW_Y - (m_btnBack.getDim().y + 10.0f) });
		m_btnBack.registerMe(this);
		m_btnDlg.adjustToFontHeadline();
		m_btnDlg.setOrigin({ Framework::STD_DRAW_X - (m_btnDlg.getDim().x + 10.0f), 10.0f });
		m_btnDlg.registerMe(this);
		m_btnMsgBox.adjustToFontHeadline();
		m_btnMsgBox.setOrigin({ Framework::STD_DRAW_X - (m_btnMsgBox.getDim().x + 10.0f), 100.0f });
		m_btnMsgBox.registerMe(this);
		m_dlgTest.setDim({ 300.0f, 300.0f });
		m_dlgTest.setCenter(Framework::getScreenCenter());
		m_dlgTest.registerMe(this);
		m_dlgTest.setZIndex(1);
	}

	virtual ~StateDev() override
	{}

	virtual bool update(float dt) override
	{
		m_dlgTest.update(dt);

		if (m_btnBack.isClicked(true))
			setNextState(TransitionState::Discard);

		if (m_btnDlg.isClicked(true))
			m_dlgTest.show();

		/*if (m_dlgTest.getResult() == UIDialog::Result::Cancel)
		{
			m_dlgTest.show();
			m_dlgTest.setDim({ 300.0f, 300.0f });
			m_dlgTest.setCenter(Framework::getScreenCenter());
		}*/

		return false;
	}

	virtual void composeFrame(Drawing& draw, float dt) override
	{
		m_btnBack.draw(draw);
		m_btnDlg.draw(draw);
		m_btnMsgBox.draw(draw);
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

	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		GameState::mouseDown(mpos, button);
		return true;
	}

	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		GameState::mouseUp(mpos, button);
		return true;
	}

	virtual bool wheel(const PointF& mpos, float amount) override
	{
		GameState::wheel(mpos, amount);
		return true;
	}

private:
	UIButtonText m_btnBack;
	UIButtonText m_btnDlg;
	UIButtonText m_btnMsgBox;
	UIDialog m_dlgTest;
};
