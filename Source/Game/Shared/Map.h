#pragma once
// Map for Client only TODO server optimization
#include "Cube.h"
#include "../../Utility/Point3.h"
#include "../../Utility/Mutex.h"
#include "../../Framework/OpenGL/VolumeTextureMap.h"
#include "CubeBase.h"
#include "../Client/MapChunk.h"
#include "MapLoader.h"
#include "../Client/MapAsset.h"
#include "GameTypes.h"

class Map : public IDrawable
{
public:
	static const size_t DEPTH = 16;
public:
	Map(GameManager& m);
	virtual ~Map();

	void setCube(Point3S pos, const CubeDesc& cd);
	void destroyBlock(const Point3S& pos);

	virtual void draw(Drawing& draw) override;
	void setDim(Point3S dim);

	Point3S getDim() const;
	std::vector<std::vector<std::pair<Point3S, CubeDesc>>> getCubeInfos() const;
	PointS getChunkSize() const;

	void loadMapAndAssets(const MapLoader::MapInfo& i);
private:
	void setCube(Point3S pos, std::unique_ptr<CubeBase> c);
private:
	std::vector<MapChunk> m_chunks;
	std::vector<MapAsset> m_assets;
	Point3S m_dim; // complete map dim
	PointS m_cdim; // dim in chunks
	VolumeTextureMap m_volumeTextureMap;

	Mutex m_muMap;
	GameManager& m_manager;
};
