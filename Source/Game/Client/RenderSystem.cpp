#include "RenderSystem.h"
#include "../Framework/Framework.h"
#include "../System/System.h"

void RenderSystem::initQueries(ManagerT & m)
{
	m.addQuery<ChunkData, Transform>();
}

void RenderSystem::begin()
{
}

void RenderSystem::tick(float dt)
{
	// move a copy to the draw thread
	auto copy = getManager().getEntsWith<ChunkData, Transform>();
	// copy above -> short lock time
	LockGuard g(m_muChunks);
	m_chunks = move(copy);
}

void RenderSystem::draw(Drawing& draw)
{
	DRAW_THREAD;

	decltype(m_chunks) chunks;
	LockGuard g(m_muChunks);
	// make a copy for short lock times (drawing will take some time)
	chunks = m_chunks;
	g.unlock();
	
	auto& shader = draw.getShaderCubeMap();
	auto& meshCube = draw.getCubeMesh();
	shader.bind();
	draw.getWaterTexture().bind(1);
	draw.getWaterfallTexture().bind(2);

	auto drawChunk = [&draw, &meshCube](std::shared_ptr<GameEntity>& c, std::unique_ptr<InstancingArray<glm::ivec3, 3, GL_FLOAT>>& a)
	{
		if(a->getDataCount())
		{
			draw.getTransform().pushModel(c->getComponent<Transform>().getMatrix());
			draw.getTransform().flush();

			a->flush();
			a->bind(2);
			meshCube.drawInstanced(a->getDataCount(), *a);

			draw.getTransform().popModel();
		}
	};

	glm::mat4 transform;
	// TODO optimize draw range
	for(auto& c : chunks)
	{
		drawChunk(c, c->getComponent<ChunkData>().m_iArray);
	}

	// draw transparency last
	draw.beginGameTransparency();
	draw.getShaderCubeTrans().bind();

	// TODO optimize draw range
	for (auto& c : chunks)
	{
		drawChunk(c, c->getComponent<ChunkData>().m_iTransArray);
	}

	draw.endGameTransparency();
	shader.unbind();
}
