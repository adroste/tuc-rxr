#pragma once
#include <array>
#include "GameTypes.h"
#include "../Utility/move_util.h"
#include "../../Framework/OpenGL/Shader/InstancingArray.h"
#include "../../Framework/OpenGL/Drawing.h"

// stores information about a 32³ block cluster for cube instancing
// Static cube chunk usage:
// - load the chunk once (should not be updated)
class StaticCubeChunk
{
public:
	static const size_t SIZE = 32;
public:
	StaticCubeChunk() = default;
	StaticCubeChunk(GameManager& m);
	virtual ~StaticCubeChunk() = default;
	MOVE_ONLY(StaticCubeChunk);

	void draw(Drawing& draw, Mesh& cube);
	void drawTransparent(Drawing& draw, Mesh& cube);

	void loadChunk(const std::vector<std::pair<Point3S, CubeDesc>>& cubes);
protected:
	std::shared_ptr<GameEntity> spawnCube(const CubeDesc& cd);
	void addCubeShape(GameEntity& e, const CubeDesc& cd);
	void addMapChunkInfo(GameEntity& e);
	// may be overwritten to obtain cube infos from neighboring chunks
	virtual std::shared_ptr<GameEntity> getCube(Point3I pos);
	bool isTransparent(const GameEntity& e);
private:
	GameManager* m_pManager = nullptr;
	// access only from main thread
	std::array<std::shared_ptr<GameEntity>, SIZE * SIZE* SIZE> m_cubes;

	// pretend that this is a float because opengl would convert it otherwise...
	// draw access -> draw thread | setData access -> main thread
	InstancingArray<glm::ivec3, 3, GL_FLOAT> m_iArray;
	InstancingArray<glm::ivec3, 3, GL_FLOAT> m_iTransArray; // array for transparent blocks

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