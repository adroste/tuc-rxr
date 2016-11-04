#pragma once
#include <string>
#include <map>
#include "CubeDesc.h"
#include <vector>
#include "../../Utility/Point3.h"

class ChunkLoader
{
public:
	ChunkLoader(const std::string& filename, const std::map<size_t, CubeDesc>& material);
	bool isOpen() const;
	static void save(const std::string& filename, const std::vector<std::pair<Point3S, size_t>>& cubes);
	const std::vector<std::pair<Point3S, CubeDesc>>& getCubes() const;
private:
	bool m_loaded = true;
	static const uint32_t s_version = 2;
	uint32_t m_version;
	size_t m_chunkDim;
	std::vector<std::pair<Point3S, CubeDesc>> m_cubes;
};
