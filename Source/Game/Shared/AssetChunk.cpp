#include "AssetChunk.h"

ChunkData::GpuArray& AssetChunk::getGpuArray()
{
	return m_gpuArray;
}

ChunkData::GpuArray& AssetChunk::getGpuTransArray()
{
	return m_gpuTransArray;
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
