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
			*
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
