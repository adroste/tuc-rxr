#pragma once
#include "../../../Framework/GameState.h"
#include "../../Shared/Game.h"

class StateGame : public GameState
{
public:
	StateGame()
		: GameState(TransitionState::Discard, false)
	{
	}
	virtual ~StateGame()
	{}

	virtual bool update(float dt) override 
	{
		// TODO find out why return value is boolean
		return false;
	}

	virtual void composeFrame(Drawing& draw, float dt) override
	{
		m_game.draw(draw);
	}

private:
	Game m_game;
};
