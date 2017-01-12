#include "MapAsset.h"

MapAsset::MapAsset(GameManager& m)
	:
m_pManager(&m),
m_chunk(m)
{
}

void MapAsset::loadChunk(const std::vector<std::pair<Point3S, CubeDesc>>& cubes)
{
	m_chunk.loadChunk(cubes);
}

void MapAsset::addInstance(glm::vec3 pos, float theta, float phi, float scale)
{
	auto phiRot = glm::rotate(phi, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec4 z = phiRot * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	m_instRot.push_back(
		
		glm::translate(pos - glm::vec3(0.0f,0.5f * (1.0f - scale),0.0f)) * glm::scale(glm::vec3(scale)) *
		glm::rotate(theta,glm::vec3(z.x,z.y,z.z)) * phiRot *
		// translate to origin
		glm::translate(glm::vec3(-float(MapChunk::SIZE / 2) + 0.5f, -float(MapChunk::SIZE), -float(MapChunk::SIZE / 2) + 0.5f))

	);
}

void MapAsset::draw(Drawing& draw, Mesh& cube, ShaderCubeMap& shader)
{
	shader.setAnimation(m_animation);
	for(const auto& m : m_instRot)
	{
		draw.getTransform().pushModel(m);
		draw.getTransform().flush();

		m_chunk.draw(draw, cube);

		draw.getTransform().popModel();
	}
	shader.setAnimation(AssetAnimation::None);
}

void MapAsset::setAnimation(AssetAnimation a)
{
	m_animation = a;
}
