#include "RenderSystem.h"
#include "../Framework/Framework.h"
#include "../System/System.h"

RenderSystem::RenderSystem(AssetLoader& al)
	:
	m_assets(al)
{
}

void RenderSystem::initQueries(ManagerT & m)
{
	m.addQuery<ChunkData, Transform>();
	m.addQuery<AssetChunkInfo, Transform>();
}

void RenderSystem::begin()
{
}

void RenderSystem::tick(float dt)
{
	// move a copy to the draw thread
	auto copy = getManager().getEntsWith<ChunkData, Transform>();
	auto copy2 = getManager().getEntsWith<AssetChunkInfo, Transform>();

	// copy above -> short lock time
	LockGuard g(m_muChunks);
	m_chunks = move(copy);
	m_assetChunks = move(copy2);
}

void RenderSystem::draw(Drawing& draw)
{
	DRAW_THREAD;

	decltype(m_chunks) chunks;
	decltype(m_assetChunks) assChunks;
	LockGuard g(m_muChunks);
	// make a copy for short lock times (drawing will take some time)
	chunks = m_chunks;
	assChunks = m_assetChunks;
	g.unlock();
	
	auto& shader = draw.getShaderCubeMap();
	auto& meshCube = draw.getCubeMesh();
	shader.bind();
	draw.getWaterTexture().bind(1);
	draw.getWaterfallTexture().bind(2);

	auto drawChunk = [&draw, &meshCube](std::shared_ptr<GameEntity>& c, InstancingArray<glm::ivec3, 3, GL_FLOAT>& a)
	{
		if(a.getDataCount())
		{
			draw.getTransform().pushModel(c->getComponent<Transform>().getMatrix());
			draw.getTransform().flush();

			a.flush();
			a.bind(2);
			meshCube.drawInstanced(a.getDataCount(), a);

			draw.getTransform().popModel();
		}
	};

	glm::mat4 transform;
	// TODO optimize draw range
	for(auto& c : chunks)
	{
		drawChunk(c, *c->getComponent<ChunkData>().m_iArray);
	}
	for(auto& c : assChunks)
	{
		drawChunk(c, m_assets.getGpuArray(c->getComponent<AssetChunkInfo>().assetID));
	}
	// bounding boxes
	/*for (auto& c : assChunks)
	{
		if (c->hasComponents<Collision, Transform>())
		{
			auto box = c->getComponent<Collision>().aabox;
			box = box.transform(c->getComponent<Transform>().getMatrix());
			draw.lineBox(box.p1, box.p2, Color::Red());
		}
	}*/

	// draw transparency last
	draw.beginGameTransparency();
	draw.getShaderCubeTrans().bind();

	// TODO optimize draw range
	for (auto& c : chunks)
	{
		drawChunk(c, *c->getComponent<ChunkData>().m_iTransArray);
	}
	for (auto& c : assChunks)
	{
		drawChunk(c, m_assets.getGpuTransArray(c->getComponent<AssetChunkInfo>().assetID));
	}

	draw.endGameTransparency();
	shader.unbind();
}
