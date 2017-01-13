#pragma once
#include "../../Utility/Point3.h"
#include <memory>
#include "../Shared/Cube.h"
#include "../../Utility/move_util.h"
#include "../../Framework/OpenGL/Shader/InstancingArray.h"
#include <array>
#include "../Shared/GameTypes.h"

class MapChunk
{
public:
	MapChunk() = default;
	MapChunk(GameManager& m);
	~MapChunk();
	MOVE_ONLY(MapChunk);

	void setCube(Point3S pos, std::shared_ptr<GameEntity>);
	void updateGpuArray();
	std::vector<std::pair<Point3S, CubeDesc>> getCubes() const;
	// note: top is mapchunk above -> mapchunk with lower y
	void setNeighbors(MapChunk* left, MapChunk* right, MapChunk* top, MapChunk* bottom);
	std::shared_ptr<GameEntity> convertFromDesc(const CubeDesc& cd) const;
private:
	// position in chunk + offset
	std::shared_ptr<GameEntity> getCube(const Point3S& p, int x, int y, int z);
	std::shared_ptr<GameEntity> getCube(const Point3S& p);
	static bool hasTranparency(const GameEntity& e);
	static bool isHidden(const GameEntity& e);
private:

	// GPU stuff
	bool m_hasChanged = true;
	bool m_hasTransparent = false;
	MapChunk* m_left = nullptr;
	MapChunk* m_right = nullptr;
	MapChunk* m_top = nullptr;
	MapChunk* m_bottom = nullptr;

	
	
	Mutex m_muCubes;
	GameManager* m_pManager = nullptr;
};
