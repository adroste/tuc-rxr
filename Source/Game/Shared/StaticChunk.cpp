#include "StaticChunk.h"

const Point3S StaticChunk::s_dim = Point3S(StaticChunk::SIZE, StaticChunk::SIZE, StaticChunk::SIZE);

void StaticChunk::begin()
{
	auto& me = getEntity();
	auto& c = me.addComponent<ChunkData>();
	c.m_iArray = std::make_unique<InstancingArray<glm::ivec3, 3, GL_FLOAT>>();
	c.m_iTransArray = std::make_unique<InstancingArray<glm::ivec3, 3, GL_FLOAT>>();
	auto& t = me.addComponent<Transform>();
	t.pos = glm::vec3(0.0f);
	t.scale = glm::vec3(0.0f);
	t.rotation = glm::vec2(0.0f);
}

void StaticChunk::draw(Drawing& draw, Mesh& cube)
{
	DRAW_THREAD;
	auto& c = getEntity().getComponent<ChunkData>();
	if(c.m_iArray->getDataCount() == 0)
		return;

	// this will create the array if it was not created
	// + it will reupload the array if changes were made
	c.m_iArray->flush();

	c.m_iArray->bind(2);
	cube.drawInstanced(c.m_iArray->getDataCount(), *c.m_iArray);
}

void StaticChunk::drawTransparent(Drawing& draw, Mesh& cube)
{
	DRAW_THREAD;
	// TODO assert transparent
	auto& c = getEntity().getComponent<ChunkData>();
	if (c.m_iTransArray->getDataCount() == 0)
		return;

	c.m_iTransArray->flush();
	c.m_iTransArray->bind(2);
	cube.drawInstanced(c.m_iTransArray->getDataCount(), *c.m_iTransArray);
}

void StaticChunk::loadChunk(const std::vector<std::pair<Point3S, CubeDesc>>& cubes)
{
	// place all cubes
	for(const auto& c : cubes)
	{
		size_t idx = s_dim.calcIndex(c.first);
		m_cubes[idx] = spawnCube(c.second,c.first);
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
					cur->getComponent<MapChunkInfo>().neighbors = 0;
					if(isTransparent(*cur))
					{
						auto upd = [x,y,z, &cur, this](int dx, int dy, int dz, MapChunkInfo::Side side)
						{
							auto o = getCube({ x + dx, y + dy, z + dz });
							if (o) cur->getComponent<MapChunkInfo>().neighbors |= side;
						};

						upd(-1, 0, 0, MapChunkInfo::Left);
						upd(0, -1, 0, MapChunkInfo::Bottom);
						upd(0, 0, -1, MapChunkInfo::Back);
						upd(1, 0, 0, MapChunkInfo::Right);
						upd(0, 1, 0, MapChunkInfo::Top);
						upd(0, 0, 1, MapChunkInfo::Front);
					}
					else
					{
						auto upd = [x,y,z, &cur, this](int dx, int dy, int dz, MapChunkInfo::Side side)
						{
							auto o = getCube({ x + dx, y + dy, z + dz });
							if (o && !isTransparent(*o))
								cur->getComponent<MapChunkInfo>().neighbors = side;
						};
						upd(-1, 0, 0, MapChunkInfo::Left);
						upd(0, -1, 0, MapChunkInfo::Bottom);
						upd(0, 0, -1, MapChunkInfo::Back);
						upd(1, 0, 0,  MapChunkInfo::Right);
						upd(0, 1, 0,  MapChunkInfo::Top);
						upd(0, 0, 1,  MapChunkInfo::Front);
					}
				}
			}
		}
	}

	// prepare data for the gpu array
	refreshGpuArray();
}

void StaticChunk::kill()
{
	m_isAlive = false;
}

void StaticChunk::tick(float dt)
{
	if(!m_isAlive)
		getEntity().kill();
}

std::shared_ptr<GameEntity> StaticChunk::spawnCube(const CubeDesc& cd, const Point3S& pos) const
{
	MAIN_THREAD;
	// spawn a dummy entity on the static thread -> no collission whatsoever
	auto e = std::make_shared<GameEntity>();
	addCubeShape(*e, cd);
	addMapChunkInfo(*e);
	return e;
}

void StaticChunk::addCubeShape(GameEntity& e, const CubeDesc& cd)
{
	auto& s = e.addComponent<CubeShape>();
	s.diffuse = cd.diffuse;
	s.gloss = cd.gloss;
	s.specular = cd.spec;
	s.shader = cd.shader;
	s.flags = cd.blockFlags;
	s.type = BlockType(cd.blockType);
}

void StaticChunk::addMapChunkInfo(GameEntity& e)
{
	e.addComponent<MapChunkInfo>().neighbors = 0;
}

std::shared_ptr<GameEntity> StaticChunk::getCube(Point3I p)
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

bool StaticChunk::isTransparent(const GameEntity& e)
{
	return e.getComponent<CubeShape>().shader != CubeShader::Default;
}

bool StaticChunk::isHidden(const GameEntity& entity)
{
	return entity.getComponent<MapChunkInfo>().neighbors == 63;
}

void StaticChunk::refreshGpuArray()
{
	MAIN_THREAD;
	std::vector<glm::ivec3> gpuArray;
	gpuArray.reserve(m_cubes.size());
	std::vector<glm::ivec3> gpuTrans;
	gpuTrans.reserve(m_cubes.size());

	size_t idx = 0;
	for(const auto& pCube : m_cubes)
	{
		if (pCube && !isHidden(*pCube))
		{
			glm::ivec3 v = { 0,0,0 };

			/*
			* x:	0-15 chunk position
			*		16-31 diffuse r+g
			*
			*	y:	0-7 diffuse b
			*		8-31 specular rgb
			*
			*	z:	0-15 gloss (int)
			*		16-18 shader type: 0 = default, 1 = transparent, 2 = water
			*		19 glowing
			*		26-31 neighbors
			*/
			v.x = idx & 0xFFFF;
			assert((idx & ~0xFFFF) == 0);

			const auto& cd = pCube->getComponent<CubeShape>();
			// Color : argb 
			v.x |= (cd.diffuse & 0xFFFF00) << 8; // 16 free bits from right
			v.y = (cd.diffuse & 0xFF);

			v.y |= (cd.specular & 0xFFFFFF) << 8;

			uint32_t gint = uint32_t(cd.gloss);
			v.z = gint & 0xFFFF;

			v.z |= (size_t(cd.shader) & 7) << 16;
			if (cd.flags & CubeDesc::Glowing)
				v.z |= size_t(1) << 19;

			v.z |= (pCube->getComponent<MapChunkInfo>().neighbors & 0x3F) << 26;

			if (isTransparent(*pCube))
				gpuTrans.push_back(v);
			else
				gpuArray.push_back(v);
		}

		idx++;
	}

	auto& c = getEntity().getComponent<ChunkData>();
	c.m_iArray->setData(move(gpuArray));
	c.m_iTransArray->setData(move(gpuTrans));
}
