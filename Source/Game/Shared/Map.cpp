#include "Map.h"
#include <assert.h>
#include "../../System/Exceptions/Exception.h"

#ifdef _CLIENT
#define CLIENTLOCK LockGuard g(m_muMap)
#else
#define CLIENTLOCK
#endif

Map::Map(Point3S dim)
	:
	m_dim(dim)
#ifdef _CLIENT
	,m_volMap(Drawing::getVolumeTextureMap())
#endif
{
	m_ppCubes = new Cube * [m_dim.size()];
	memset(m_ppCubes, 0, sizeof(Cube*) * m_dim.size());

#ifdef _CLIENT
	m_volMap.init(dim);
#endif
}

Map::~Map()
{
	if (!m_ppCubes) return;

	for (size_t i = 0; i < m_dim.size(); ++i)
	{
		if(m_ppCubes[i])
		{
			delete m_ppCubes[i];
			m_ppCubes[i] = nullptr;
		}
	}
	delete[] m_ppCubes;
	m_ppCubes = nullptr;
}

void Map::setCube(Cube* cube, bool isLight, bool overwrite)
{
	CLIENTLOCK;

	size_t idx = getIndex(Point3S(cube->getPos()));
	if(overwrite && m_ppCubes[idx])
	{
		delete m_ppCubes[idx];
		m_ppCubes[idx] = nullptr;
	}
	assert(m_ppCubes[idx] == nullptr);
	if (m_ppCubes[idx])
		throw Exception("Map::setCube array position not empty");
	m_ppCubes[idx] = cube;

#ifdef _CLIENT
	m_volMap.setValue(Point3S(cube->getPos()),isLight? 0.0f : 1.0f);
#endif
}

void Map::destroyBlock(const Point3S& pos)
{
	CLIENTLOCK;

	size_t idx = getIndex(pos);
	if(m_ppCubes[idx])
	{
		delete m_ppCubes[idx];
		m_ppCubes[idx] = nullptr;
#ifdef _CLIENT
		m_volMap.setValue(pos, 0.0f);
#endif
	}
}
#ifdef _CLIENT
void Map::draw(Drawing& draw)
{
	CLIENTLOCK;

	if(!m_texCreated)
	{
		// TODO resolve this
		draw.setMapInfo(m_dim);
		m_texCreated = true;
	}
	m_volMap.bind(0);

	for (Cube** i = m_ppCubes, **end = m_ppCubes + m_dim.size(); i != end; ++i)
	{
		if (*i)
		{
			(*i)->draw(draw);
		}
	}
}

void Map::setDim(Point3S dim)
{
	if (dim == m_dim)
		return;

	CLIENTLOCK;
	
	Cube** newCubes = new Cube*[dim.size()];
	memset(newCubes, 0, dim.size() * sizeof(Cube*));

	// move prev cubes
	for(size_t x = 0; x < std::min(dim.x,m_dim.x); x++)
		for (size_t y = 0; y < std::min(dim.y, m_dim.y); y++)
			for (size_t z = 0; z < std::min(dim.z, m_dim.z); z++)
			{
				auto idx = getIndex({ x,y,z });
				newCubes[dim.width * (y + z * dim.height) + x] = m_ppCubes[idx];
				m_ppCubes[idx] = nullptr;
			}

	// remove unused cubes if map is smaller
	for(size_t i = 0; i < m_dim.size(); i++)
	{
		delete m_ppCubes[i];
		m_ppCubes[i] = nullptr;
	}

	delete[] m_ppCubes;
	m_ppCubes = newCubes;
	m_dim = dim;

	m_volMap.resize(m_dim);
	m_texCreated = false;
}
#endif // _CLIENT

Point3S Map::getDim() const
{
	return m_dim;
}

std::vector<std::pair<CubeDesc, Point3S>> Map::getCubeInfos()
{
	std::vector<std::pair<CubeDesc, Point3S>> c;
	for (Cube** i = m_ppCubes, **end = m_ppCubes + m_dim.size(); i != end; ++i)
	{
		if (*i)
		{
			c.push_back(std::make_pair((*i)->getDesc(),Point3S((*i)->getPos())));
		}
	}
	return c;
}

size_t Map::getIndex(Point3S pos) const
{
	assert(pos.x < m_dim.x);
	assert(pos.y < m_dim.y);
	assert(pos.z < m_dim.z);
	return m_dim.width * (pos.y + pos.z * m_dim.height) + pos.x;
}
