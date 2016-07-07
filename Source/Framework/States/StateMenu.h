#pragma once
#include "GameState.h"

class StateMenu : GameState
{
public:
	virtual void update(float dt) override;

	virtual void composeFrame(float dt) override;

};