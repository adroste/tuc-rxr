#pragma once
#include "GameTypes.h"
#include "UniformGrid.h"

class MovementSystem : public GameSystem
{
public:
	MovementSystem(UniformGrid& grid);
	virtual ~MovementSystem() override = default;
	virtual void initQueries(ManagerT& m) override;
	//virtual void begin() override;
	virtual void tick(float dt) override;

	virtual void onEntitySpawn(std::shared_ptr<EntityT> e) override;
	virtual void onEntityDeath(std::shared_ptr<EntityT> e) override;

private:
	UniformGrid& m_grid;
};
