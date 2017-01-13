#include "CubeChunk.h"

static Point3S s_dim = Point3S(StaticCubeChunk::SIZE, StaticCubeChunk::SIZE, StaticCubeChunk::SIZE);

StaticCubeChunk::StaticCubeChunk(GameManager & m)
	:
m_pManager(&m)
{

}

void StaticCubeChunk::draw(Drawing& draw, Mesh& cube)
{
	DRAW_THREAD;
	// TODO update gpu array
	if(m_iArray.getDataCount() == 0)
		return;

	// this will create the array if it was not created
	// + it will reupload the array if changes were made
	m_iArray.flush();

	m_iArray.bind(2);
	cube.drawInstanced(m_iArray.getDataCount(), m_iArray);
}

void StaticCubeChunk::drawTransparent(Drawing& draw, Mesh& cube)
{
	DRAW_THREAD;
	// TODO assert transparent
	// TODO upload
	if (m_iTransArray.getDataCount() == 0)
		return;

	m_iTransArray.flush();
	m_iTransArray.bind(2);
	cube.drawInstanced(m_iTransArray.getDataCount(), m_iTransArray);
}

void StaticCubeChunk::loadChunk(const std::vector<std::pair<Point3S, CubeDesc>>& cubes)
{
	// place all cubes
	for(const auto& c : cubes)
	{
		size_t idx = s_dim.calcIndex(c.first);
		m_cubes[idx] = spawnCube(c.second);
	}
	// determine occluded faces
	for(int x = 0; x < SIZE; x++)
	{
		for (int y = 0; y < SIZE; y++)
		{
			for (int z = 0; z < SIZE; z++)
			{
				auto cur = getCube({x,y,z});
				if(cur)
				{
					// set neighboring flags for this cube
					
				}
			}
		}
	}
}

std::shared_ptr<GameEntity> StaticCubeChunk::spawnCube(const CubeDesc& cd)
{
	MAIN_THREAD;
	auto e = m_pManager->addEntity();
	addCubeShape(*e, cd);
	addMapChunkInfo(*e);
	return e;
}

void StaticCubeChunk::addCubeShape(GameEntity& e, const CubeDesc& cd)
{
	auto& s = e.addComponent<CubeShape>();
	s.diffuse = cd.diffuse;
	s.gloss = cd.gloss;
	s.specular = cd.spec;
	s.shader = cd.shader;
	s.flags = cd.blockFlags;
	s.type = BlockType(cd.blockType);
}

void StaticCubeChunk::addMapChunkInfo(GameEntity& e)
{
	e.addComponent<MapChunkInfo>().neighbors = 0;
}

std::shared_ptr<GameEntity> StaticCubeChunk::getCube(Point3I p)
{
	if(p.x >= 0 && p.x < SIZE &&
		p.y >= 0 && p.y < SIZE &&
		p.z >= 0 && p.z < SIZE)
	{
		auto idx = s_dim.calcIndex(p);
		return m_cubes[idx];
	}
	return std::shared_ptr<GameEntity>();
}
