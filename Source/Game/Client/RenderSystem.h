#pragma once
#include "../Shared/GameTypes.h"
#include "../../Framework/OpenGL/Drawing.h"

class RenderSystem : public GameSystem
{
public:
	virtual ~RenderSystem() override = default;
	virtual void initQueries(ManagerT& m) override;
	virtual void begin() override;
	virtual void tick(float dt) override;
	// execute draw logic on draw thead
	void draw(Drawing& draw);
private:
	Mutex m_muChunks;
	std::vector<std::shared_ptr<GameEntity>> m_chunks;
};
