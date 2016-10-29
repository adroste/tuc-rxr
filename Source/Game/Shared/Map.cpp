#include "Map.h"
#include <assert.h>
#include "../../System/Exceptions/Exception.h"

Map::Map(Point3S dim)
	:
	m_dim(Point3S(0,0,0))
{
	setDim(dim);
}

Map::~Map()
{
	
}

void Map::setCube(Point3S pos, std::unique_ptr<CubeBase> c)
{
	// TODO lock mutex
	// find chunk
	size_t chunk = pos.x / 16;
	assert(chunk < m_chunks.size());
	pos.x -= chunk * 16;
	m_chunks[chunk].setCube(pos, move(c));
}

void Map::destroyBlock(const Point3S& pos)
{
	setCube(pos, nullptr);
}

void Map::draw(Drawing& draw)
{
	
}

void Map::setDim(Point3S dim)
{
	// TODO lock mutex
	// allocate / deallocate chunks
	size_t nCurChunks = m_chunks.size();
	size_t nNeededChunks = (dim.x + 15) / 16;

	Point3S chSize = Point3S(16, dim.height, dim.depth);
	if(nCurChunks == nNeededChunks)
	{
		// just resize
		for (auto& c : m_chunks)
			c.resize(dim.height, dim.depth);
	}
	else if(nNeededChunks < nCurChunks)
	{
		// shrink
		while (nNeededChunks > m_chunks.size())
			m_chunks.pop_back();
	}
	else
	{
		// expand
		while (nNeededChunks > m_chunks.size())
			m_chunks.push_back(MapChunk(chSize));
	}
}

Point3S Map::getDim() const
{
	return m_dim;
}

std::vector<std::pair<CubeDesc, Point3S>> Map::getCubeInfos()
{
	return{};
}

std::shared_ptr<Map::MaterialInfo> Map::addMaterial(const CubeMaterial& m)
{

}
