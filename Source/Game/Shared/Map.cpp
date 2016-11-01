#include "Map.h"
#include <assert.h>
#include "../../System/Exceptions/Exception.h"

Map::Map(Point3S dim)
	:
	m_dim(Point3S(0, 0, 0))
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
	PointS chunk = PointS(pos.x, pos.y) / 32;
	assert(chunk.x * chunk.y < m_chunks.size());
	pos.x -= chunk.x * 32;
	pos.y -= chunk.y * 32;
	m_chunks[chunk.y * (m_dim.x / 32) + chunk.x].setCube(pos, move(c));
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

	auto& meshCube = draw.getCubeMesh();

	glm::mat4 transform;
	shader.bind();
	m_volumeTextureMap.bind(0);

	for (auto& c : m_chunks)
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
	assert(dim.z == 32);
	// TODO lock mutex
	// allocate / deallocate chunks
	size_t nx = (dim.x + 31) / 32;
	size_t ny = (dim.y + 31) / 32;

	if (nx * 32 == m_dim.x && ny * 32== m_dim.y)
		return;

	std::vector<MapChunk> newChunks;
	newChunks.reserve(nx * ny);

	for (size_t x = 0; x < nx; x++)
	{
		for (size_t y = 0; y < ny; y++)
		{
			if(x < m_dim.x / 32 && y < m_dim.y / 32)
			{
				// use old chunk
				newChunks.push_back(std::move(m_chunks.at(y * (m_dim.x / 32) + x)));
			}
			else newChunks.emplace_back();
		}
	}

	dim.x = nx * 32;
	dim.y = nx * 32;
	dim.z = 32;
	m_chunks = move(newChunks);
	m_volumeTextureMap.resize(dim);
	m_dim = dim;
}

Point3S Map::getDim() const
{
	return m_dim;
}

std::vector<std::pair<CubeDesc, Point3S>> Map::getCubeInfos()
{
	return {};
}
