#pragma once
#include "StaticChunk.h"

// relevant data for asset cubes
struct AssetCubeDesc
{
	CubeShape shape;
	MapChunkInfo chunkInfo;
};

class AssetChunk : public StaticChunk<std::shared_ptr<AssetCubeDesc>>
{
public:
	AssetChunk() = default;
	virtual ~AssetChunk() override = default;
	MOVE_ONLY(AssetChunk);
protected:
	virtual std::shared_ptr<AssetCubeDesc> spawnCube(const CubeDesc& cd, const Point3S& pos) const override;
	virtual void setGpuData(std::vector<glm::ivec3>& solid, std::vector<glm::ivec3>& trans) override;
	virtual CubeShape& getCubeShape(std::shared_ptr<AssetCubeDesc>& c) override final;
	virtual MapChunkInfo& getChunkInfo(std::shared_ptr<AssetCubeDesc>& c) override final;
private:
	ChunkData::GpuArray m_gpuArray;
	ChunkData::GpuArray m_gpuTransArray;
};
