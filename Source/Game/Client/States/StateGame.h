#pragma once
#include "../../../Framework/GameState.h"
#include "../../Shared/Game.h"

class StateGame : public GameState
{
public:
	StateGame()
	{
		m_game.registerMe(this);
	}
	virtual ~StateGame()
	{}

	virtual bool update(float dt) override 
	{
		return false;
	}

	virtual void composeFrame(Drawing& draw, float dt) override
	{
		m_game.draw(draw);

		draw.getUiCam().apply(draw);
		Font& f = draw.getFont(Font::Style::Text, Font::Size::M);

		f.setColor(Color::White());
		f.write("hello", { 10,10 });
	}

private:
	Game m_game;
};
