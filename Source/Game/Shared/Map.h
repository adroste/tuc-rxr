#pragma once
// Map for Client only TODO server optimization
#include "Cube.h"
#include "../../Utility/Point3.h"
#include "../../Utility/Mutex.h"
#include "../../Framework/OpenGL/VolumeTextureMap.h"
#include "MapLoader.h"
#include "GameTypes.h"
#include "MapChunk.h"

class Map
{
public:
	static const size_t DEPTH = 16;
public:
	Map(GameManager& m);
	virtual ~Map();

	void setCube(Point3S pos, const CubeDesc& cd);
	void destroyBlock(Point3S pos);

	virtual void bind(Drawing& draw);
	void setDim(Point3S dim);

	Point3S getDim() const;
	std::vector<std::vector<std::pair<Point3S, CubeDesc>>> getCubeInfos() const;
	PointS getChunkSize() const;

	void loadMapAndAssets(const MapLoader::MapInfo& i);

	void update();
	// function for the editor
	void dispose();
private:
	// pointers to the map chunk script (the chunk itself is an entity)
	std::vector<std::shared_ptr<MapChunk>> m_chunks;
	Point3S m_dim; // complete map dim
	PointS m_cdim; // dim in chunks
	VolumeTextureMap m_volumeTextureMap;

	Mutex m_muMap;
	GameManager& m_manager;
};
