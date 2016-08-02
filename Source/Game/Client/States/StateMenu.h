#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Framework/Sound/Sound.h"
#include "../../../Framework/UI/UIButton.h"
#include "StateGame.h"
#include "../../../Framework/UI/UIButtonText.h"
#include "../../../Framework/OpenGL/Shader/UIColorPicker.h"
#include "../../../Framework/UI/UIInputField.h"
#include "../../../Framework/UI/UILabel.h"

class StateMenu : public GameState
{
public:
	StateMenu()
		:
		GameState(GameState::TransitionState::ForcePreserve),
		m_title(Drawing::getFont(Font::Style::Headline,Font::Size::L),"MainWindow"),
		m_btnSingle(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Can't Touch This"),
		m_btnEdit(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Editor"),
		m_colorPicker(300, { 640, 360 }),
		m_inp(Drawing::getFont(Font::Style::Text,Font::Size::M),20)
	{
		//m_btnSingle.setDim({ 350, 80 });
		m_btnSingle.adjustToFontHeadline();
		m_btnSingle.centerX(200.0f);
		m_btnEdit.adjustToFontHeadline();
		m_btnEdit.setWidth(m_btnSingle.getDim().x);
		m_btnEdit.centerX(300.0f);

		m_inp.setDim({ 300,60 });
		m_inp.setOrigin({ 10,10 });

		m_title.centerX(50.0f);

		m_btnSingle.registerMe(this);
		m_btnEdit.registerMe(this);
		m_inp.registerMe(this);

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
			setNextState(std::unique_ptr<GameState>(new StateGame()));
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
	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		handled = GameState::mouseMove(mpos, handled);


		m_mpos = mpos;
		return handled;
	}
private:
	UILabel	m_title;

	PointI m_mpos;
	UIButtonText m_btnSingle;
	UIButtonText m_btnEdit;

	UIColorPicker m_colorPicker;
};
