#pragma once
#include "../../Utility/Point3.h"
#include <memory>
#include "../Shared/Cube.h"
#include "../Shared/CubeBase.h"
#include "../../Utility/move_util.h"

class MapChunk
{
public:
	MapChunk(Point3S dim);
	~MapChunk();
	MOVE_ONLY(MapChunk);

	void setCube(Point3S pos, std::unique_ptr<CubeBase> c);
	void updateGpuArray();
	void resize(size_t height, size_t depth);
private:
	Point3S m_dim;
	std::vector<std::unique_ptr<CubeBase>> m_cubes;

	// GPU stuff
	bool m_hasChanged = true;
	std::vector<glm::ivec4> m_gpuArray; // xy = index of chunk wz = index of material
};
