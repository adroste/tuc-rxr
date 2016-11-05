#include "MapAsset.h"

MapAsset::MapAsset()
{
}

void MapAsset::loadChunk(const std::vector<std::pair<Point3S, CubeDesc>>& cubes)
{
	m_chunk.loadChunk(cubes);
}

void MapAsset::addInstance(glm::vec3 pos, float theta, float phi, float scale)
{
	// TODO add rotation
	// TODO fix displacement
	m_instRot.push_back(
		
		glm::translate(pos + glm::vec3(0.5f)/* + glm::vec3(0.0f,0.55f,0.0f)*/) * glm::scale(glm::vec3(scale)) *
		// translate to origin
		glm::translate(glm::vec3(-float(MapChunk::SIZE / 2), -float(MapChunk::SIZE), -float(MapChunk::SIZE / 2)))

	);
}

void MapAsset::draw(Drawing& draw, Mesh& cube)
{
	for(const auto& m : m_instRot)
	{
		draw.getTransform().pushModel(m);
		draw.getTransform().flush();

		m_chunk.draw(draw, cube);

		draw.getTransform().popModel();
	}
}
