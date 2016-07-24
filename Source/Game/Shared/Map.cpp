#include "Map.h"
#include <assert.h>


Map::Map(size_t width, size_t height, size_t depth)
	:
	m_width(width),
	m_height(height),
	m_depth(depth)
{
	m_ppCubes = new Cube*[m_width * m_height * m_depth];
	memset(m_ppCubes, 0, sizeof(Cube*) * m_width * m_height * m_depth);
}

Map::~Map()
{
	if (!m_ppCubes) return;

	for (size_t i = 0; i < m_width * m_height * m_depth; ++i)
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

void Map::setCube(size_t x, size_t y, size_t z, Cube* cube)
{
	size_t idx = getIndex(x, y, z);
	assert(m_ppCubes[idx] == nullptr);
	m_ppCubes[idx] = cube;
}

#ifdef _CLIENT
void Map::draw(Drawing& draw)
{
	for (Cube** i = m_ppCubes, **end = m_ppCubes + m_width * m_height * m_depth; i != end; ++i)
	{
		if (*i)
		{
			(*i)->draw(draw);
		}
	}
}
#endif // _CLIENT

size_t Map::getWidth() const
{
	return m_width;
}

size_t Map::getHeight() const
{
	return m_height;
}

size_t Map::getDepth() const
{
	return m_depth;
}

size_t Map::getIndex(size_t x, size_t y, size_t z) const
{
	assert(x < m_width);
	assert(y < m_height);
	assert(z < m_depth);
	return m_width * (y + z * m_height) + x;
}
