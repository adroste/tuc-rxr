#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Framework/Sound/Sound.h"
#include "../../../Framework/UI/UIButton.h"
#include "StateGame.h"
#include "../../../Framework/UI/UIButtonText.h"
#include "../../../Framework/UI/UIDialogColorPicker.h"
#include "../../../Framework/UI/UIInputField.h"
#include "../../../Framework/UI/UILabel.h"
#include "StateEditor.h"

class StateMenu : public GameState
{
public:
	StateMenu()
		:
		m_title(Drawing::getFont(Font::Style::Headline,Font::Size::L),"MainWindow"),
		m_btnSingle(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Can't Touch This"),
		m_btnEdit(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Editor"),
		m_colorPicker(300, { 640, 360 })
	{
		//m_btnSingle.setDim({ 350, 80 });
		m_btnSingle.adjustToFontHeadline();
		m_btnSingle.centerX(200.0f);
		m_btnEdit.adjustToFontHeadline();
		m_btnEdit.setWidth(m_btnSingle.getDim().x);
		m_btnEdit.centerX(300.0f);

		m_title.centerX(50.0f);

		m_btnSingle.registerMe(this);
		m_btnEdit.registerMe(this);

		m_mpos = { 500,500 };
		Sound::playMusic(Sound::Music::Hey);
	}
	virtual ~StateMenu()
	{}

	virtual bool update(float dt) override
	{
		if (m_btnSingle.isClicked(true))
		{	
			Sound::playMusic(Sound::Music::Theme);
			//Sound::playSound(Sound::Sfx::Plop);
			setNextState(TransitionState::ForcePreserve, std::unique_ptr<GameState>(new StateGame()));
		}

		if (m_btnEdit.isClicked(true))
		{
			setNextState(TransitionState::ForcePreserve, std::unique_ptr<GameState>(new StateEditor()));
		}

		return false;
	}

	virtual void composeFrame(Drawing& draw, float dt) override
	{
	
		//m_colorPicker.draw(draw);
		m_title.draw(draw);

		m_btnSingle.draw(draw);
		m_btnEdit.draw(draw);
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

		m_mpos = mpos;
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
	UILabel	m_title;

	PointF m_mpos;
	UIButtonText m_btnSingle;
	UIButtonText m_btnEdit;

	UIDialogColorPicker m_colorPicker;
};
