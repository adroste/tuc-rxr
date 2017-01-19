#pragma once
#include "MapLoader.h"
#include "GameTypes.h"
#include "AssetChunk.h"

class AssetLoader
{
public:
	AssetLoader(GameManager& m);
	void loadAssetData(const MapLoader::AssetInfo& i);
	void loadAssetsData(const std::vector<MapLoader::AssetInfo>& in);
	ChunkData::GpuArray& getGpuArray(size_t id);
	ChunkData::GpuArray& getGpuTransArray(size_t id);
private:
	static void setTransform(GameEntity& e, const glm::vec3& pos, const glm::vec3& scale, float phi, float theta);
private:
	GameManager& m_manager;
	std::vector<std::unique_ptr<AssetChunk>> m_assets;
};
