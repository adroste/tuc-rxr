#include "AssetChunk.h"

ChunkData::GpuArray& AssetChunk::getGpuArray()
{
	return m_gpuArray;
}

ChunkData::GpuArray& AssetChunk::getGpuTransArray()
{
	return m_gpuTransArray;
}

AABox AssetChunk::calcAABox() const
{
	int xmin = SIZE;
	int ymin = SIZE;
	int zmin = SIZE;
	int xmax = -1;
	int ymax = -1;
	int zmax = -1;

	for(size_t x = 0; x < SIZE; x++)
	{
		for (size_t y = 0; y < SIZE; y++)
		{
			for (size_t z = 0; z < SIZE; z++)
			{
				if(m_cubes[s_dim.calcIndex({x,y,z})])
				{
					// cube is here
					xmin = std::min(xmin, int(x));
					ymin = std::min(ymin, int(y));
					zmin = std::min(zmin, int(z));
					xmax = std::max(xmax, int(x));
					ymax = std::max(ymax, int(y));
					zmax = std::max(zmax, int(z));
				}
			}
		}
	}

	AABox r;
	r.xmin = float(xmin) - 0.5f;
	r.ymin = float(ymin) - 0.5f;
	r.zmin = float(zmin) - 0.5f;
	r.xmax = float(xmax) + 0.5f;
	r.ymax = float(ymax) + 0.5f;
	r.zmax = float(zmax) + 0.5f;
	return r;
}

std::shared_ptr<AssetCubeDesc> AssetChunk::spawnCube(const CubeDesc & cd, const Point3S & pos) const
{
	auto p = std::make_shared<AssetCubeDesc>();
	setCubeShape(p->shape, cd);
	setChunkInfo(p->chunkInfo);
	return p;
}

void AssetChunk::setGpuData(std::vector<glm::ivec3>& solid, std::vector<glm::ivec3>& trans)
{
	MAIN_THREAD;
	m_gpuArray.setData(solid);
	m_gpuTransArray.setData(trans);
}

CubeShape& AssetChunk::getCubeShape(std::shared_ptr<AssetCubeDesc>& c)
{
	return c->shape;
}

MapChunkInfo& AssetChunk::getChunkInfo(std::shared_ptr<AssetCubeDesc>& c)
{
	return c->chunkInfo;
}
