#pragma once
#include <array>
#include "GameTypes.h"
#include "../Utility/move_util.h"
#include "../../Framework/OpenGL/Shader/InstancingArray.h"
#include "../../Framework/OpenGL/Drawing.h"
#include <algorithm>

static const size_t CHUNK_SIZE = 32;
// stores information about a 32³ block cluster for cube instancing
// Static cube chunk usage:
// - load the chunk once (should not be updated)
template <class T>
class StaticChunk
{
public:
	static const size_t SIZE = CHUNK_SIZE;
protected:
	static const Point3S s_dim;
public:
	StaticChunk() = default;
	virtual ~StaticChunk() = default;
	MOVE_ONLY(StaticChunk);

	void loadChunk(const std::vector<std::pair<Point3S, CubeDesc>>& cubes);
protected:
	virtual T spawnCube(const CubeDesc& cd, const Point3S& pos) const = 0;
	virtual void setGpuData(std::vector<glm::ivec3>& solid, std::vector<glm::ivec3>& trans) = 0;
	static void setCubeShape(CubeShape& s, const CubeDesc& cd)
	{
		s.diffuse = cd.diffuse;
		s.gloss = cd.gloss;
		s.specular = cd.spec;
		s.shader = cd.shader;
		s.flags = cd.blockFlags;
		s.type = BlockType(cd.blockType);
	}
	static void setChunkInfo(MapChunkInfo& i)
	{
		i.neighbors = 0;
	}
	virtual CubeShape& getCubeShape(T& c) = 0;
	virtual MapChunkInfo& getChunkInfo(T& c) = 0;

	// may be overwritten to obtain cube infos from neighboring chunks
	virtual T getCube(Point3I pos);
	bool isHidden(T& c);
	void refreshGpuArray();
	bool isTransparent(T& c);
protected:
	// access only from main thread
	std::array<T, SIZE * SIZE* SIZE> m_cubes;
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
};

template <class T>
const Point3S StaticChunk<T>::s_dim = Point3S(StaticChunk::SIZE, StaticChunk::SIZE, StaticChunk::SIZE);

template <class T>
void StaticChunk<T>::loadChunk(const std::vector<std::pair<Point3S, CubeDesc>>& cubes)
{
	// place all cubes
	for (const auto& c : cubes)
	{
		size_t idx = s_dim.calcIndex(c.first);
		m_cubes[idx] = spawnCube(c.second, c.first);
	}
	// determine occluded faces
	for (int x = 0; x < SIZE; x++)
	{
		for (int y = 0; y < SIZE; y++)
		{
			for (int z = 0; z < SIZE; z++)
			{
				auto cur = getCube({ x,y,z });
				if (cur)
				{
					// set neighboring flags for this cube
					getChunkInfo(cur).neighbors = 0;
					if (isTransparent(cur))
					{
						auto upd = [x, y, z, &cur, this](int dx, int dy, int dz, MapChunkInfo::Side side)
						{
							auto o = getCube({ x + dx, y + dy, z + dz });
							if (o) getChunkInfo(cur).neighbors |= side;
						};

						upd(-1, 0, 0, MapChunkInfo::Left);
						upd(0, -1, 0, MapChunkInfo::Bottom);
						upd(0, 0, -1, MapChunkInfo::Back);
						upd(1, 0, 0, MapChunkInfo::Right);
						upd(0, 1, 0, MapChunkInfo::Top);
						upd(0, 0, 1, MapChunkInfo::Front);
					}
					else
					{
						auto upd = [x, y, z, &cur, this](int dx, int dy, int dz, MapChunkInfo::Side side)
						{
							auto o = getCube({ x + dx, y + dy, z + dz });
							if (o && !isTransparent(o))
								getChunkInfo(cur).neighbors = side;
						};
						upd(-1, 0, 0, MapChunkInfo::Left);
						upd(0, -1, 0, MapChunkInfo::Bottom);
						upd(0, 0, -1, MapChunkInfo::Back);
						upd(1, 0, 0, MapChunkInfo::Right);
						upd(0, 1, 0, MapChunkInfo::Top);
						upd(0, 0, 1, MapChunkInfo::Front);
					}
				}
			}
		}
	}

	// prepare data for the gpu array
	refreshGpuArray();
}

template <class T>
T StaticChunk<T>::getCube(Point3I p)
{
	if (p.x >= 0 && p.x < SIZE &&
		p.y >= 0 && p.y < SIZE &&
		p.z >= 0 && p.z < SIZE)
	{
		auto idx = s_dim.calcIndex(p);
		return m_cubes[idx];
	}
	return T();
}

template <class T>
bool StaticChunk<T>::isHidden(T& c)
{
	return getChunkInfo(c).neighbors == 63;
}

template <class T>
void StaticChunk<T>::refreshGpuArray()
{
	MAIN_THREAD;
	std::vector<glm::ivec3> gpuArray;
	gpuArray.reserve(m_cubes.size());
	std::vector<glm::ivec3> gpuTrans;
	gpuTrans.reserve(m_cubes.size());

	size_t idx = 0;
	for (auto& pCube : m_cubes)
	{
		if (pCube && !isHidden(pCube))
		{
			glm::ivec3 v = { 0,0,0 };

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

			const auto& cd = getCubeShape(pCube);
			// Color : argb 
			v.x |= (cd.diffuse & 0xFFFF00) << 8; // 16 free bits from right
			v.y = (cd.diffuse & 0xFF);

			v.y |= (cd.specular & 0xFFFFFF) << 8;

			uint32_t gint = uint32_t(cd.gloss);
			v.z = gint & 0xFFFF;

			v.z |= (size_t(cd.shader) & 7) << 16;
			if (cd.flags & CubeDesc::Glowing)
				v.z |= size_t(1) << 19;

			v.z |= (getChunkInfo(pCube).neighbors & 0x3F) << 26;

			if (isTransparent(pCube))
				gpuTrans.push_back(v);
			else
				gpuArray.push_back(v);
		}

		idx++;
	}

	setGpuData(gpuArray, gpuTrans);
}

template <class T>
bool StaticChunk<T>::isTransparent(T& c)
{
	return getCubeShape(c).shader != CubeShader::Default;
}
