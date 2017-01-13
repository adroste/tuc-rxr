#include "MapChunk.h"
#include <bitset>
#include "../../Framework/Framework.h"

const Point3S MapChunk::m_dim = { SIZE,SIZE,SIZE };

MapChunk::MapChunk(GameManager& m)
	:
m_pManager(&m)
{
	// initialize cubes
	auto s = m_dim.size();
}

MapChunk::~MapChunk()
{
}

void MapChunk::setCube(Point3S pos, std::shared_ptr<GameEntity> c)
{
	MAIN_THREAD;
	auto index = m_dim.calcIndex(pos);

	// check for neighbors
	std::shared_ptr<GameEntity> ch = nullptr;
	if (c)
	{
		c->getComponent<MapChunkInfo>().neighbors = 0;
		if (!hasTranparency(*c))
		{
			// no transparency
#define UPD(dx,dy,dz,side, oside) ch = getCube(pos, dx, dy, dz); if (ch && \
			!hasTranparency(*c)) \
			{c->getComponent<MapChunkInfo>().neighbors |= MapChunkInfo::side; ch->getComponent<MapChunkInfo>().neighbors |= MapChunkInfo::oside;} \
			else if(ch) {/*has transparency*/ \
			ch->getComponent<MapChunkInfo>().neighbors |= MapChunkInfo::oside;}

			UPD(-1, 0, 0, Left, Right);
			UPD(0, -1, 0, Bottom, Top);
			UPD(0, 0, -1, Back, Front);
			UPD(1, 0, 0, Right, Left);
			UPD(0, 1, 0, Top, Bottom);
			UPD(0, 0, 1, Front, Back);
#undef UPD
		}
		else // has Transparency
		{
#define UPD(dx,dy,dz,side, oside) ch = getCube(pos, dx, dy, dz); if (ch) \
	{c->getComponent<MapChunkInfo>().neighbors |= MapChunkInfo::side; if(hasTranparency(*c)) ch->getComponent<MapChunkInfo>().neighbors |= MapChunkInfo::oside; else ch->getComponent<MapChunkInfo>().neighbors &= ~MapChunkInfo::oside;}
			UPD(-1, 0, 0, Left, Right);
			UPD(0, -1, 0, Bottom, Top);
			UPD(0, 0, -1, Back, Front);
			UPD(1, 0, 0, Right, Left);
			UPD(0, 1, 0, Top, Bottom);
			UPD(0, 0, 1, Front, Back);
#undef UPD
		}
	}
	else
	{
		// delete block
		if (m_cubes[index])
		{
			// clear flags around block
#define UPD(dx,dy,dz,oside) ch = getCube(pos, dx, dy, dz); if (ch) \
	{ch->getComponent<MapChunkInfo>().neighbors &= ~MapChunkInfo::oside;}
			UPD(-1, 0, 0, Right);
			UPD(0, -1, 0, Top);
			UPD(0, 0, -1, Front);
			UPD(1, 0, 0, Left);
			UPD(0, 1, 0, Bottom);
			UPD(0, 0, 1, Back);
#undef UPD
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
	std::vector<glm::ivec3> gpuTrans;
	if (m_hasTransparent)
		gpuTrans.reserve(m_cubes.size());

	LockGuard g(m_muCubes);
	size_t idx = 0;
	for (const auto& pCube : m_cubes)
	{
		if (pCube && !isHidden(*pCube))
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
			*		16-18 shader type: 0 = default, 1 = transparent, 2 = water
			*		19 glowing
			*		26-31 neighbors
			*/
			v.x = idx & 0xFFFF;
			assert((idx & ~0xFFFF) == 0);

			const auto& cd = pCube->getComponent<CubeShape>();
			// Color : argb 
			v.x |= (cd.diffuse & 0xFFFF00) << 8; // 16 free bits from right
			v.y = (cd.diffuse & 0xFF);

			v.y |= (cd.specular & 0xFFFFFF) << 8;

			uint32_t gint = uint32_t(cd.gloss);
			v.z = gint & 0xFFFF;

			v.z |= (size_t(cd.shader) & 7) << 16;
			if (cd.flags & CubeDesc::Glowing)
				v.z |= size_t(1) << 19;

			v.z |= (pCube->getComponent<MapChunkInfo>().neighbors & 0x3F) << 26;

			assert(m_hasTransparent || !hasTranparency(*pCube));
			if (hasTranparency(*pCube))
				gpuTrans.push_back(v);
			else
				gpuArray.push_back(v);
		}

		idx++;
	}

	m_iArray.setData(move(gpuArray));
	if (m_hasTransparent)
	{
		// draw front to back
		m_iTransArray.setData(move(gpuTrans));
	}
	m_hasChanged = false;
}

std::vector<std::pair<Point3S, CubeDesc>> MapChunk::getCubes() const
{
	MAIN_THREAD;
	std::vector<std::pair<Point3S, CubeDesc>> d;
	size_t idx = 0;
	for (const auto& c : m_cubes)
	{
		if (c)
		{
			CubeDesc des;
			const auto& s = c->getComponent<CubeShape>();
			des.shader = s.shader;
			des.blockFlags = s.flags;
			des.blockType = uint8_t(s.type);
			des.diffuse = s.diffuse;
			des.gloss = s.gloss;
			des.spec = s.specular;


			// TODO add hp information
			d.push_back(std::make_pair(m_dim.fromIndex(idx), des));
		}
		idx++;
	}
	return d;
}

void MapChunk::setNeighbors(MapChunk* left, MapChunk* right, MapChunk* top, MapChunk* bottom)
{
	MAIN_THREAD;

	m_left = left;
	m_right = right;
	m_top = top;
	m_bottom = bottom;
}

std::shared_ptr<GameEntity> MapChunk::getCube(const Point3S& p, int dx, int dy, int dz)
{
	MAIN_THREAD;
	int x = int(p.x) + dx;
	int y = int(p.y) + dy;
	int z = int(p.z) + dz;
	// only one offset may differ from zero
	assert(((dx != 0) + (dy != 0) + (dz != 0)) == 1);

	if (z < 0 || z >= SIZE)
		return nullptr;

	auto curChunk = this;

	if(x < 0) // left chunk?
	{
		curChunk = m_left;
		x += SIZE;
	} else if(x >= SIZE)
	{
		curChunk = m_right;
		x -= SIZE;
	}

	if(y < 0)
	{
		curChunk = m_top;
		y += SIZE;
	} else if(y >= SIZE)
	{
		curChunk = m_bottom;
		y -= SIZE;
	}

	assert(x >= 0);
	assert(x < SIZE);
	assert(y >= 0);
	assert(y < SIZE);

	if (curChunk == nullptr)
		return nullptr; // no block present

	// get Block
	return curChunk->getCube(Point3S(x, y, z));
}

std::shared_ptr<GameEntity> MapChunk::getCube(const Point3S& p)
{
	MAIN_THREAD;
	return m_cubes[m_dim.calcIndex(p)];
}

bool MapChunk::hasTranparency(const GameEntity& e)
{
	return e.getComponent<CubeShape>().shader != CubeShader::Default;
}

bool MapChunk::isHidden(const GameEntity& e)
{
	return e.getComponent<MapChunkInfo>().neighbors == 63;
}
