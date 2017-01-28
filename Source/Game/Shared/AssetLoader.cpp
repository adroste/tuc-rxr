#include "AssetLoader.h"

AssetLoader::AssetLoader(GameManager& m)
	:
	m_manager(m)
{
}

void AssetLoader::loadAssetData(const MapLoader::AssetInfo & i)
{
	// create asset
	m_assets.emplace_back(new AssetChunk());
	size_t id = m_assets.size() - 1;
	auto& chunk = m_assets.back();
	chunk->loadChunk(i.geometry);
	chunk->setAnimation(i.animation);

	// spawn instances
	for(const auto& inst : i.instances)
	{
		auto e = m_manager.addEntity();
		// add components
		setTransform(*e, inst.pos, glm::vec3(inst.scale), inst.phi, inst.theta);
		setMovement(*e);
		e->addComponent<AssetChunkInfo>().assetID = id;
		setCollision(*e, *chunk);
	}
}

void AssetLoader::loadAssetsData(const std::vector<MapLoader::AssetInfo>& in)
{
	for (auto& d : in)
		loadAssetData(d);
}

ChunkData::GpuArray& AssetLoader::getGpuArray(size_t id)
{
	return m_assets.at(id)->getGpuArray();
}

ChunkData::GpuArray& AssetLoader::getGpuTransArray(size_t id)
{
	return m_assets.at(id)->getGpuTransArray();
}

void AssetLoader::setTransform(GameEntity& e, const glm::vec3& pos, const glm::vec3& scale, float phi, float theta)
{
	auto& t = e.addComponent<Transform>();
	t.pos = pos;
	t.scale = scale;
	t.rotation = glm::vec2(phi, theta);
}

void AssetLoader::setMovement(GameEntity& e)
{
	auto& m = e.addComponent<Movement>();
	m.velocity = glm::vec3(0.0f);
	m.acceleration = glm::vec3(0.0f, 6.5f, 0.0f);
}

void AssetLoader::setCollision(GameEntity& e, AssetChunk& c)
{
	e.addComponent<Collision>().aabox = c.calcAABox();
}
