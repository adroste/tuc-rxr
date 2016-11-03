#include "MapChunk.h"
#include <bitset>

const Point3S MapChunk::m_dim = { SIZE,SIZE,SIZE };

MapChunk::MapChunk()
{
	// initialize cubes
	auto s = m_dim.size();
	m_cubes.reserve(s);
	for (size_t i = 0; i < s; i++)
		m_cubes.push_back(std::unique_ptr<CubeBase>(nullptr));
}

MapChunk::~MapChunk()
{
}

void MapChunk::draw(Drawing& draw, Mesh& cube)
{
	updateGpuArray(); // TODO move this function somewhere else?
	if (m_iArray.getDataCount() == 0)
		return;
	// create array or reupload if data changed
	m_iArray.flush();

	// draw instanced
	m_iArray.bind(2);

	cube.drawInstanced(m_iArray.getDataCount(), m_iArray);
}

void MapChunk::setCube(Point3S pos, std::unique_ptr<CubeBase> c)
{
	auto index = m_dim.calcIndex(pos);

	// check for neighbors
	if(c)
	{
		c->neighbors = 0;
		if(c->isOpaque())
		{
			if (pos.x > 0)
				if (isOpaque(Point3S(pos.x - 1, pos.y, pos.z)))
					c->neighbors |= CubeBase::Left;

			if (pos.y > 0)
				if (isOpaque(Point3S(pos.x, pos.y - 1, pos.z)))
					c->neighbors |= CubeBase::Bottom;

			if (pos.z > 0)
				if (isOpaque(Point3S(pos.x, pos.y, pos.z - 1)))
					c->neighbors |= CubeBase::Back;

			if (pos.x < m_dim.x - 1)
				if (isOpaque(Point3S(pos.x + 1, pos.y, pos.z)))
					c->neighbors |= CubeBase::Right;

			if (pos.y < m_dim.y - 1)
				if (isOpaque(Point3S(pos.x, pos.y + 1, pos.z)))
					c->neighbors |= CubeBase::Top;

			if (pos.z < m_dim.z - 1)
				if (isOpaque(Point3S(pos.x, pos.y, pos.z + 1)))
					c->neighbors |= CubeBase::Front;
		}
		else
		{
			if (pos.x > 0)
				if (getCube(Point3S(pos.x - 1, pos.y, pos.z)))
					c->neighbors |= CubeBase::Left;

			if (pos.y > 0)
				if (getCube(Point3S(pos.x, pos.y - 1, pos.z)))
					c->neighbors |= CubeBase::Bottom;

			if (pos.z > 0)
				if (getCube(Point3S(pos.x, pos.y, pos.z - 1)))
					c->neighbors |= CubeBase::Back;

			if (pos.x < m_dim.x - 1)
				if (getCube(Point3S(pos.x + 1, pos.y, pos.z)))
					c->neighbors |= CubeBase::Right;

			if (pos.y < m_dim.y - 1)
				if (getCube(Point3S(pos.x, pos.y + 1, pos.z)))
					c->neighbors |= CubeBase::Top;

			if (pos.z < m_dim.z - 1)
				if (getCube(Point3S(pos.x, pos.y, pos.z + 1)))
					c->neighbors |= CubeBase::Front;
		}
		

		updateNeighborFlags(pos, c->isOpaque(), true);
	}
	else
	{
		// delete block
		if(m_cubes[index])
		{
			// clear flags around block
			updateNeighborFlags(pos, false, false);
		}
	}

	LockGuard g(m_muCubes);

	m_cubes[index] = move(c);
	m_hasChanged = true;
}

void MapChunk::updateGpuArray()
{
	if (!m_hasChanged)
		return;

	std::vector<glm::ivec3> gpuArray;
	gpuArray.reserve(m_cubes.size());

	LockGuard g(m_muCubes);
	size_t idx = 0;
	for (const auto& pCube : m_cubes)
	{
		if (pCube)
		{
			glm::ivec3 v = {0,0,0};

			/*
			* x:	0-15 chunk position
			*		16-31 diffuse r+g
			*
			*	y:	0-7 diffuse b
			*		8-31 specular rgb
			*
			*	z:	0-15 gloss (int)
			*		16-23 shader type: 0 = default, 1 = transparent, 2 = water
			*		26-31 neighbors
			*/
			v.x = idx & 0xFFFF;
			assert((idx & ~0xFFFF) == 0);

			const auto& cd = pCube->getDesc();
			// Color : argb 
			v.x |= (cd.diffuse & 0xFFFF00) << 8; // 16 free bits from right
			v.y = (cd.diffuse & 0xFF);

			v.y |= (cd.spec & 0xFFFFFF) << 8;

			uint32_t gint = uint32_t(cd.gloss);
			v.z = gint & 0xFFFF;

			v.z |= (size_t(cd.shader) & 0xFF) << 16;
			v.z |= (pCube->neighbors & 0x3F) << 26;

			gpuArray.push_back(v);
		}

		idx++;
	}

	m_iArray.setData(move(gpuArray));
}

void MapChunk::appendCubeDescs(std::vector<std::pair<CubeDesc, Point3S>>& d) const
{
	size_t idx = 0;
	for(const auto& c : m_cubes)
	{
		if(c)
		{
			d.push_back(std::make_pair<CubeDesc, Point3S>(CubeDesc(c->getDesc()), m_dim.fromIndex(idx)));
		}
		idx++;
	}
}

std::unique_ptr<CubeBase>& MapChunk::getCube(const Point3S& p)
{
	return m_cubes[m_dim.calcIndex(p)];
}

void MapChunk::updateNeighborFlags(const Point3S& pos, bool isOpaque, bool isBlock)
{
	if(isOpaque)
	{
		if (pos.x > 0)
		{
			auto& c = getCube(Point3S(pos.x - 1, pos.y, pos.z));
			if (c) c->neighbors |= CubeBase::Right;
		}

		if (pos.y > 0)
		{
			auto& c = getCube(Point3S(pos.x, pos.y - 1, pos.z));
			if (c) c->neighbors |= CubeBase::Top;
		}

		if (pos.z > 0)
		{
			auto& c = getCube(Point3S(pos.x, pos.y, pos.z - 1));
			if (c) c->neighbors |= CubeBase::Front;
		}

		if (pos.x < m_dim.x - 1)
		{
			auto& c = getCube(Point3S(pos.x + 1, pos.y, pos.z));
			if (c) c->neighbors |= CubeBase::Left;
		}

		if (pos.y < m_dim.y - 1)
		{
			auto& c = getCube(Point3S(pos.x, pos.y + 1, pos.z));
			if (c) c->neighbors |= CubeBase::Bottom;
		}

		if (pos.z < m_dim.z - 1)
		{
			auto& c = getCube(Point3S(pos.x, pos.y, pos.z + 1));
			if (c) c->neighbors |= CubeBase::Back;
		}
	}
	else
	{
		// its not opaque, but is it a block?
		if (pos.x > 0)
		{
			auto& c = getCube(Point3S(pos.x - 1, pos.y, pos.z));
			if (c)
				if(isBlock && !c->isOpaque())
					c->neighbors |= CubeBase::Right;
				else c->neighbors &= ~CubeBase::Right;
		}

		if (pos.y > 0)
		{
			auto& c = getCube(Point3S(pos.x, pos.y - 1, pos.z));
			if (c) 
				if (isBlock && !c->isOpaque())
				c->neighbors |= CubeBase::Top;
				else c->neighbors &= ~CubeBase::Top;
		}

		if (pos.z > 0)
		{
			auto& c = getCube(Point3S(pos.x, pos.y, pos.z - 1));
			if (c) 
				if (isBlock && !c->isOpaque())
					c->neighbors |= CubeBase::Front;
				else c->neighbors &= ~CubeBase::Front;
		}

		if (pos.x < m_dim.x - 1)
		{
			auto& c = getCube(Point3S(pos.x + 1, pos.y, pos.z));
			if (c) 
				if (isBlock && !c->isOpaque())
					c->neighbors |= CubeBase::Left;
				else c->neighbors &= ~CubeBase::Left;
		}

		if (pos.y < m_dim.y - 1)
		{
			auto& c = getCube(Point3S(pos.x, pos.y + 1, pos.z));
			if (c)
				if (isBlock && !c->isOpaque())
					c->neighbors |= CubeBase::Bottom;
				else c->neighbors &= ~CubeBase::Bottom;
		}

		if (pos.z < m_dim.z - 1)
		{
			auto& c = getCube(Point3S(pos.x, pos.y, pos.z + 1));
			if (c) 
				if (isBlock && !c->isOpaque())
					c->neighbors |= CubeBase::Back;
				else c->neighbors &= ~CubeBase::Back;
		}
	}
}

bool MapChunk::isOpaque(const Point3S& p) const
{
	auto& b = m_cubes[m_dim.calcIndex(p)];
	if (b)
		return b->isOpaque();
	return false;
}
