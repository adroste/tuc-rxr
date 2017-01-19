#pragma once
#include "../Shared/GameTypes.h"
#include "../../Framework/OpenGL/Drawing.h"
#include "../Shared/AssetLoader.h"

class RenderSystem : public GameSystem
{
public:
	virtual ~RenderSystem() override = default;
	explicit RenderSystem(AssetLoader& al);
	virtual void initQueries(ManagerT& m) override;
	virtual void begin() override;
	virtual void tick(float dt) override;
	// execute draw logic on draw thead
	void draw(Drawing& draw);
private:
	Mutex m_muChunks;
	AssetLoader& m_assets;
	std::vector<std::shared_ptr<GameEntity>> m_chunks;
	std::vector<std::shared_ptr<GameEntity>> m_assetChunks;
};
