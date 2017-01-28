#include "MapChunk.h"


MapChunk::MapChunk(const Point3S& chunkPos)
	:
	m_chunkPos(chunkPos)
{

}

void MapChunk::begin()
{
	auto& me = getEntity();
	auto& c = me.addComponent<ChunkData>();
	c.m_iArray = std::make_unique<InstancingArray<glm::ivec3, 3, GL_FLOAT>>();
	c.m_iTransArray = std::make_unique<InstancingArray<glm::ivec3, 3, GL_FLOAT>>();
	auto& t = me.addComponent<Transform>();
	t.pos = glm::vec3(0.0f);
	t.scale = glm::vec3(1.0f);
	t.rotation = glm::vec2(0.0f);
	// update transform omponent
	setChunkPosition(m_chunkPos);
}

void MapChunk::setNeighbors(MapChunk* left, MapChunk* right, MapChunk* top, MapChunk* bottom)
{
	MAIN_THREAD;

	m_left = left;
	m_right = right;
	m_top = top;
	m_bottom = bottom;
}

void MapChunk::setCube(const Point3S& pos, const CubeDesc* cd)
{
	MAIN_THREAD;
	std::shared_ptr<GameEntity> c;
	if(cd) c = spawnCube(*cd, pos);
	auto index = s_dim.calcIndex(pos);

	// check for neighbors
	if(c)
	{
		c->getComponent<MapChunkInfo>().neighbors = 0;
		if(!isTransparent(c))
		{
			// is solid
			auto upd = [this, pos, &c](int dx, int dy, int dz, MapChunkInfo::Side side, MapChunkInfo::Side oside)
			{
				auto ch = getCube(Point3I(pos) + Point3I(dx, dy, dz));
				if (ch)
					if (!isTransparent(ch))
					{
						c->getComponent<MapChunkInfo>().neighbors |= side;
						ch->getComponent<MapChunkInfo>().neighbors |= oside;
					}
					else // is transparent
						ch->getComponent<MapChunkInfo>().neighbors |= oside;
			};

			upd(-1, 0, 0, MapChunkInfo::Left,	MapChunkInfo::Right);
			upd(0, -1, 0, MapChunkInfo::Bottom, MapChunkInfo::Top);
			upd(0, 0, -1, MapChunkInfo::Back,	MapChunkInfo::Front);
			upd(1, 0, 0,  MapChunkInfo::Right,  MapChunkInfo::Left);
			upd(0, 1, 0,  MapChunkInfo::Top,	MapChunkInfo::Bottom);
			upd(0, 0, 1,  MapChunkInfo::Front,  MapChunkInfo::Back);
		}
		else
		{
			// is transparent
			auto upd = [this, pos, &c](int dx, int dy, int dz, MapChunkInfo::Side side, MapChunkInfo::Side oside)
			{
				auto ch = getCube(Point3I(pos) + Point3I(dx, dy, dz));
				if(ch)
				{
					c->getComponent<MapChunkInfo>().neighbors |= side;
					if (isTransparent(ch))
						ch->getComponent<MapChunkInfo>().neighbors |= oside;
					else
						ch->getComponent<MapChunkInfo>().neighbors &= ~oside;
				}
			};
			upd(-1, 0, 0, MapChunkInfo::Left, MapChunkInfo::Right);
			upd(0, -1, 0, MapChunkInfo::Bottom, MapChunkInfo::Top);
			upd(0, 0, -1, MapChunkInfo::Back, MapChunkInfo::Front);
			upd(1, 0, 0, MapChunkInfo::Right, MapChunkInfo::Left);
			upd(0, 1, 0, MapChunkInfo::Top, MapChunkInfo::Bottom);
			upd(0, 0, 1, MapChunkInfo::Front, MapChunkInfo::Back);
		}
	}
	else if(m_cubes[index])
	{
		// delete block
		auto upd = [this,pos](int dx, int dy, int dz, MapChunkInfo::Side side)
		{
			auto ch = getCube(Point3I(pos) + Point3I(dx, dy, dz));
			if (ch) ch->getComponent<MapChunkInfo>().neighbors &= ~side;
		};
		upd(-1, 0, 0, MapChunkInfo::Right);
		upd(0, -1, 0, MapChunkInfo::Top);
		upd(0, 0, -1, MapChunkInfo::Front);
		upd(1, 0, 0, MapChunkInfo::Left);
		upd(0, 1, 0, MapChunkInfo::Bottom);
		upd(0, 0, 1, MapChunkInfo::Back);
	}
	if (m_cubes[index]) m_cubes[index]->kill();
	m_cubes[index] = c;
	// has changed
	m_changed = true;
}

void MapChunk::setChunkPosition(const Point3S& p)
{
	m_chunkPos = p;
	getEntity().getComponent<Transform>().pos = glm::vec3(float(p.x * SIZE), float(p.y * SIZE), float(p.z * SIZE));
}

std::vector<std::pair<Point3S, CubeDesc>> MapChunk::getCubes() const
{
	MAIN_THREAD;
	std::vector<std::pair<Point3S, CubeDesc>> d;
	size_t idx = 0;
	for(const auto& c : m_cubes)
	{
		if(c)
		{
			CubeDesc des;
			const auto& s = c->getComponent<CubeShape>();
			des.shader = s.shader;
			des.blockFlags = s.flags;
			des.blockType = uint8_t(s.type);
			des.diffuse = s.diffuse;
			des.gloss = s.gloss;
			des.spec = s.specular;

			// TODO add hp information
			d.push_back(std::make_pair(s_dim.fromIndex(idx), des));
		}
		idx++;
	}
	return d;
}

void MapChunk::tick(float dt)
{
	MAIN_THREAD;
	if (!m_isAlive)
		getEntity().kill();

	if (m_changed)
	{
		refreshGpuArray();
		m_changed = false;
	}
}

void MapChunk::kill()
{
	m_isAlive = false;
}

// modified to contain neighbor chunk information
std::shared_ptr<GameEntity> MapChunk::getCube(Point3I pos)
{
	if (pos.z < 0 || pos.z >= SIZE)
		return nullptr;

	auto curChunk = this;
	if (pos.x < 0) // left chunk?
	{
		curChunk = m_left;
		pos.x += SIZE;
	}
	else if (pos.x >= SIZE)
	{
		curChunk = m_right;
		pos.x -= SIZE;
	}

	if (pos.y < 0)
	{
		curChunk = m_top;
		pos.y += SIZE;
	}
	else if (pos.y >= SIZE)
	{
		curChunk = m_bottom;
		pos.y -= SIZE;
	}

	assert(pos.x >= 0);
	assert(pos.x < SIZE);
	assert(pos.y >= 0);
	assert(pos.y < SIZE);

	if (curChunk == nullptr)
		return nullptr; // no block present

	return curChunk->m_cubes[s_dim.calcIndex(pos)];
}

std::shared_ptr<GameEntity> MapChunk::spawnCube(const CubeDesc& cd, const Point3S& pos) const
{
	MAIN_THREAD;
	auto e = getManager().addEntity();
	addCubeShape(*e, cd);
	addMapChunkInfo(*e);
	addTransform(*e, pos);
	addCollision(*e);
	return e;
}

void MapChunk::setGpuData(std::vector<glm::ivec3>& solid, std::vector<glm::ivec3>& trans)
{
	auto& c = getEntity().getComponent<ChunkData>();
	c.m_iArray->setData(move(solid));
	c.m_iTransArray->setData(move(trans));
}

void MapChunk::addTransform(GameEntity& e, const Point3S& pos) const
{
	auto& t = e.addComponent<Transform>();
	t.pos = glm::vec3(	float(m_chunkPos.x * s_dim.x + pos.x),
						float(m_chunkPos.y * s_dim.y + pos.y),
						float(m_chunkPos.z * s_dim.z + pos.z));
	t.rotation = glm::vec2(0.0f);
	t.scale = glm::vec3(1.0f);
}

CubeShape& MapChunk::getCubeShape(std::shared_ptr<GameEntity>& c)
{
	return c->getComponent<CubeShape>();
}

MapChunkInfo& MapChunk::getChunkInfo(std::shared_ptr<GameEntity>& c)
{
	return c->getComponent<MapChunkInfo>();
}

void MapChunk::addCubeShape(GameEntity& e, const CubeDesc& cd)
{
	setCubeShape(e.addComponent<CubeShape>(), cd);
}

void MapChunk::addMapChunkInfo(GameEntity& e)
{
	setChunkInfo(e.addComponent<MapChunkInfo>());
}

void MapChunk::addCollision(GameEntity& e)
{
	auto& c = e.addComponent<StaticCollision>();
	// make the bounding box a little smaller so that map cubes fit in exactly one grid spot
	c.aabox = AABox(glm::vec3(-0.49f), glm::vec3(0.49f));
}
