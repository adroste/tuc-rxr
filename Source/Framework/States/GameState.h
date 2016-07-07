#pragma once
#include "../../System/Input.h"

class GameState : Input::Receiver
{
public:
	GameState() {}
	virtual ~GameState() {}

	virtual void update(float dt) = 0;
	virtual void composeFrame(float dt) = 0;
};