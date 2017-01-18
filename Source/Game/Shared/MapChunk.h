#pragma once
#include "StaticChunk.h"

// special chunk for Cubes within map (collission per block)
class MapChunk : public StaticChunk<std::shared_ptr<GameEntity>>, public GameScript
{
public:
	MapChunk() = default;
	// chunk position in chunk coordinates (real position 0,32,0 => chunk 0,1,0)
	MapChunk(const Point3S& chunkPos);
	MOVE_ONLY(MapChunk);
	virtual ~MapChunk() override = default;

	virtual void begin() override;
	void setNeighbors(MapChunk* left, MapChunk* right, MapChunk* top, MapChunk* bottom);
	// pass nullptr to delete block
	void setCube(const Point3S& pos, const CubeDesc* cd);
	void setChunkPosition(const Point3S& p);
	// editor extension
	std::vector<std::pair<Point3S, CubeDesc>> getCubes() const;
	// this will upload new blocks
	virtual void tick(float dt) override;
	void kill();
protected:
	virtual std::shared_ptr<GameEntity> getCube(Point3I pos) override;
	virtual std::shared_ptr<GameEntity> spawnCube(const CubeDesc& cd, const Point3S& pos) const override final;
	virtual void setGpuData(std::vector<glm::ivec3>& solid, std::vector<glm::ivec3>& trans) override final;
	void addTransform(GameEntity& e, const Point3S& pos) const;
private:
	Point3S m_chunkPos;
	// neighboring chunks
	MapChunk* m_left = nullptr;
	MapChunk* m_right = nullptr;
	MapChunk* m_top = nullptr;
	MapChunk* m_bottom = nullptr;
	bool m_changed = false;
	bool m_isAlive = true;
};