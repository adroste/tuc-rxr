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

void Map::setCube(Point3S pos, const CubeDesc& cd)
{
	// construct cube
	// at the moment just default cubes
	auto pCube = std::unique_ptr<CubeBase>(new CubeBase(cd));
	m_volumeTextureMap.setValue(pos, pCube->getShadow());
	setCube(pos, move(pCube));
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
	m_volumeTextureMap.setValue(pos, 0.0f);
}

void Map::draw(Drawing& draw)
{
	draw.setMapInfo(m_dim);
	// enable volume map
	if (!m_volumeTextureMap.isCreated())
		m_volumeTextureMap.create();

	auto& shader = draw.getShaderCubeMap();
	shader.setChunkHeight(m_dim.height);

	auto& meshCube = draw.getCubeMesh();

	glm::mat4 transform;
	shader.bind();
	m_volumeTextureMap.bind(0);

	for(auto& c : m_chunks)
	{
		draw.getTransform().pushModel(transform);
		draw.getTransform().flush();
		c.draw(draw, meshCube);
		draw.getTransform().popModel();
		transform = glm::translate(transform, glm::vec3(16.0f, 0.0f, 0.0f));
	}
	shader.unbind();
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

	m_volumeTextureMap.resize(dim);
	m_dim = dim;
}

Point3S Map::getDim() const
{
	return m_dim;
}

std::vector<std::pair<CubeDesc, Point3S>> Map::getCubeInfos()
{
	return{};
}