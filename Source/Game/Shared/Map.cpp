#include "Map.h"
#include <assert.h>


Map::Map(size_t width, size_t height, size_t depth)
	:
	Map(Point3S(width, height, depth))
{}

Map::Map(Point3S dim)
	:
	m_dim(dim)
{
	m_ppCubes = new Cube * [m_dim.size()];
	memset(m_ppCubes, 0, sizeof(Cube*) * m_dim.size());
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

void Map::setCube(Cube* cube, bool overwrite)
{
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
}

#ifdef _CLIENT
void Map::draw(Drawing& draw)
{
	for (Cube** i = m_ppCubes, **end = m_ppCubes + m_dim.size(); i != end; ++i)
	{
		if (*i)
		{
			(*i)->draw(draw);
		}
	}
}
#endif // _CLIENT

Point3S Map::getDim() const
{
	return m_dim;
}

size_t Map::getIndex(Point3S pos) const
{
	assert(pos.x < m_dim.x);
	assert(pos.y < m_dim.y);
	assert(pos.z < m_dim.z);
	return m_dim.width * (pos.y + pos.z * m_dim.height) + pos.x;
}
