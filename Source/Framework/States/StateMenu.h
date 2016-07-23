#pragma once
#include "GameState.h"
#include "../UI/UIButton.h"
#include "../Sound/Sound.h"

class StateMenu : public GameState
{
public:
	StateMenu()
		:
		GameState(GameState::TransitionState::ForcePreserve)
	{
		m_myButt.setDim({ 300, 100 });
		m_myButt.center();

		m_myButt.registerMe(this);
		//Sound::playMusic(Sound::Music::Hey);
	}
	virtual ~StateMenu()
	{}

	virtual bool update(float dt) override
	{
		if (m_myButt.isClicked(true))
			Sound::playMusic(Sound::Music::Theme);
			//Sound::playSound(Sound::Sfx::Plop);

		return false;
	}

	virtual void composeFrame(Drawing& draw, float dt) override
	{
		m_myButt.draw(draw);

		//draw.rect(RectF::constructFromPoint(m_mpos, 10.0f), Color::Red());

		draw.coloredCube(m_mpos, 15.0f, Color::Cyan());
		draw.coloredCube(m_mpos + PointF(30.0f,0.0f), 15.0f, Color::Cyan());
		
		draw.getFont(Font::Style::Text, Font::Size::L).setColor(Color::White());
		draw.getFont(Font::Style::Text, Font::Size::L).write("Sample", { 100,100 });
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
