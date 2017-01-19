#pragma once
#include "GameTypes.h"

class MovementSystem : public GameSystem
{
public:

	virtual ~MovementSystem() override = default;
	virtual void initQueries(ManagerT& m) override;
	//virtual void begin() override;
	virtual void tick(float dt) override;
};
