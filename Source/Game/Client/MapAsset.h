#pragma once
#include "MapChunk.h"

class MapAsset
{
public:
	MOVE_ONLY(MapAsset);
	MapAsset();
	void loadChunk(const std::vector<std::pair<Point3S, CubeDesc>>& cubes);
	void addInstance(glm::vec3 pos, float theta, float phi, float scale);
	void draw(Drawing& draw, Mesh& cube);
private:
	MapChunk m_chunk;
	std::vector<glm::mat4> m_instRot; // rotation for instance
};
