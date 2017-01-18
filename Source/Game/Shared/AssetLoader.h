#pragma once
#include "MapLoader.h"
#include "GameTypes.h"

class AssetLoader
{
public:
	AssetLoader(GameManager& m);
	void loadAssetData(const MapLoader::AssetInfo& i);
	void loadAssetsData(const std::vector<MapLoader::AssetInfo>& in);

private:
	GameManager& m_manager;
};
