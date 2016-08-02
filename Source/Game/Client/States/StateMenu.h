#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Framework/Sound/Sound.h"
#include "../../../Framework/UI/UIButton.h"
#include "StateGame.h"
#include "../../../Framework/UI/UIButtonText.h"
#include "../../../Framework/OpenGL/Shader/UIColorPicker.h"
#include "../../../Framework/UI/UIInputField.h"

class StateMenu : public GameState
{
public:
	StateMenu()
		:
		GameState(GameState::TransitionState::ForcePreserve),
		m_myButt(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::L), "Can't Touch This"),
		m_colorPicker(300, { 640, 360 }),
		m_inp(Drawing::getFont(Font::Style::Text,Font::Size::M),20)
	{
		//m_myButt.setDim({ 350, 80 });
		m_myButt.adjustToFontHeadline();
		m_myButt.center();

		m_inp.setDim({ 300,60 });
		m_inp.setOrigin({ 10,10 });

		m_myButt.registerMe(this);
		m_inp.registerMe(this);

		m_mpos = { 500,500 };
		Sound::playMusic(Sound::Music::Hey);
	}
	virtual ~StateMenu()
	{}

	virtual bool update(float dt) override
	{
		if (m_myButt.isClicked(true))
		{	
			Sound::playMusic(Sound::Music::Theme);
			//Sound::playSound(Sound::Sfx::Plop);
			setNextState(std::unique_ptr<GameState>(new StateGame()));
		}

		return false;
	}

	virtual void composeFrame(Drawing& draw, float dt) override
	{
		//m_myButt.draw(draw);
		//m_colorPicker.draw(draw);

		draw.line({ 640, 360 }, m_mpos, 100.0f, Color::Cyan());

		draw.rect(RectF::constructFromPoint(m_mpos, 10.0f), Color::Red());


		draw.getFont(Font::Style::Text, Font::Size::L).setColor(Color::White());
		draw.getFont(Font::Style::Text, Font::Size::L).write("Sample", { 100,100 });
		m_inp.draw(draw);
	}
	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		handled = GameState::mouseMove(mpos, handled);


		m_mpos = mpos;
		return handled;
	}
private:
	PointI m_mpos;
	UIButtonText m_myButt;
	UIInputField m_inp;


	UIColorPicker m_colorPicker;
};
