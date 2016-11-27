#include "MapChunk.h"
#include <bitset>
#include "../../Framework/Framework.h"

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
	DRAW_THREAD;
	
	updateGpuArray(); // TODO move this function somewhere else?
	if (m_iArray.getDataCount() == 0)
		return;
	// create array or reupload if data changed
	m_iArray.flush();

	// draw instanced
	m_iArray.bind(2);

	cube.drawInstanced(m_iArray.getDataCount(), m_iArray);
}

void MapChunk::drawTransparent(Drawing& draw, Mesh& cube)
{
	DRAW_THREAD;
	assert(m_hasTransparent);
	// gpu data was updated before

	if (m_iTransArray.getDataCount() == 0)
		return;

	m_iTransArray.flush();

	m_iTransArray.bind(2);

	cube.drawInstanced(m_iTransArray.getDataCount(), m_iTransArray);
}

void MapChunk::setCube(Point3S pos, std::unique_ptr<CubeBase> c)
{
	MAIN_THREAD;
	auto index = m_dim.calcIndex(pos);

	// check for neighbors
	CubeBase* ch = nullptr;
	if(c)
	{
		c->neighbors = 0;
		if (!c->hasTransparency())
		{
			// no transparency
#define UPD(dx,dy,dz,side, oside) ch = getCube(pos, dx, dy, dz); if (ch && !ch->hasTransparency()) \
	{c->neighbors |= CubeBase::side; ch->neighbors |= CubeBase::oside;} else if(ch) {/*has transparency*/ \
			ch->neighbors |= CubeBase::oside;}

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
	{c->neighbors |= CubeBase::side; if(ch->hasTransparency()) ch->neighbors |= CubeBase::oside; else ch->neighbors &= ~CubeBase::oside;}
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
		if(m_cubes[index])
		{
			// clear flags around block
#define UPD(dx,dy,dz,oside) ch = getCube(pos, dx, dy, dz); if (ch) \
	{ch->neighbors &= ~CubeBase::oside;}
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
		if (pCube && !pCube->isHidden())
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

			const auto& cd = pCube->getDesc();
			// Color : argb 
			v.x |= (cd.diffuse & 0xFFFF00) << 8; // 16 free bits from right
			v.y = (cd.diffuse & 0xFF);

			v.y |= (cd.spec & 0xFFFFFF) << 8;

			uint32_t gint = uint32_t(cd.gloss);
			v.z = gint & 0xFFFF;

			v.z |= (size_t(cd.shader) & 7) << 16;
			if (cd.blockFlags & CubeDesc::Glowing)
				v.z |= size_t(1) << 19;

			v.z |= (pCube->neighbors & 0x3F) << 26;

			assert(m_hasTransparent || !pCube->hasTransparency());
			if (pCube->hasTransparency())
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
		/*decltype(gpuTrans) v2;
		v2.reserve(gpuTrans.size());
		for (auto r = gpuTrans.rbegin(), end = gpuTrans.rend(); r != end; ++r)
			v2.push_back(*r);

		m_iTransArray.setData(move(v2));*/
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
			d.push_back(std::make_pair(m_dim.fromIndex(idx), CubeDesc(c->getDesc())));
		}
		idx++;
	}
	return d;
}

void MapChunk::loadChunk(const std::vector<std::pair<Point3S, CubeDesc>>& cubes)
{
	MAIN_THREAD;
	for(const auto& c : cubes)
	{
		// TODO improve
		setCube(c.first, std::unique_ptr<CubeBase>(new CubeBase(c.second)));
	}
}

void MapChunk::setTransparency(bool hasTrans)
{
	MAIN_THREAD;

	m_hasTransparent = hasTrans;
}

void MapChunk::setNeighbors(MapChunk* left, MapChunk* right, MapChunk* top, MapChunk* bottom)
{
	MAIN_THREAD;

	m_left = left;
	m_right = right;
	m_top = top;
	m_bottom = bottom;
}

CubeBase* MapChunk::getCube(const Point3S& p, int dx, int dy, int dz)
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

CubeBase* MapChunk::getCube(const Point3S& p)
{
	MAIN_THREAD;
	return m_cubes[m_dim.calcIndex(p)].get();
}