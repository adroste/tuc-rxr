#pragma once
#include <array>
#include "GameTypes.h"
#include "../Utility/move_util.h"
#include "../../Framework/OpenGL/Shader/InstancingArray.h"
#include "../../Framework/OpenGL/Drawing.h"
#include <algorithm>

// stores information about a 32³ block cluster for cube instancing
// Static cube chunk usage:
// - load the chunk once (should not be updated)
class StaticChunk : public GameScript
{
public:
	static const size_t SIZE = 32;
protected:
	static const Point3S s_dim;
public:
	StaticChunk() = default;
	virtual ~StaticChunk() = default;
	MOVE_ONLY(StaticChunk);


	virtual void begin() override;

	void draw(Drawing& draw, Mesh& cube);
	void drawTransparent(Drawing& draw, Mesh& cube);

	void loadChunk(const std::vector<std::pair<Point3S, CubeDesc>>& cubes);
	// will be killed in the next iteration
	void kill();
	virtual void tick(float dt) override;
protected:
	virtual std::shared_ptr<GameEntity> spawnCube(const CubeDesc& cd, const Point3S& pos) const;
	static void addCubeShape(GameEntity& e, const CubeDesc& cd);
	static void addMapChunkInfo(GameEntity& e);
	// may be overwritten to obtain cube infos from neighboring chunks
	virtual std::shared_ptr<GameEntity> getCube(Point3I pos);
	static bool isTransparent(const GameEntity& e);
	static bool isHidden(const GameEntity& entity);
	void refreshGpuArray();
protected:
	// access only from main thread
	std::array<std::shared_ptr<GameEntity>, SIZE * SIZE* SIZE> m_cubes;
	bool m_isAlive = true;
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
};