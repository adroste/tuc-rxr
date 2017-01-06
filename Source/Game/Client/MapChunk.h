#pragma once
#include "../../Utility/Point3.h"
#include <memory>
#include "../Shared/Cube.h"
#include "../Shared/CubeBase.h"
#include "../../Utility/move_util.h"
#include "../../Framework/OpenGL/Shader/InstancingArray.h"

class MapChunk
{
public:
	static const int SIZE = 32;

public:
	MapChunk();
	~MapChunk();
	MOVE_ONLY(MapChunk);

	void draw(Drawing& draw, Mesh& cube);
	void drawTransparent(Drawing& draw, Mesh& cube);
	void setCube(Point3S pos, std::unique_ptr<CubeBase> c);
	void updateGpuArray();
	std::vector<std::pair<Point3S, CubeDesc>> getCubes() const;
	void loadChunk(const std::vector<std::pair<Point3S, CubeDesc>>& cubes);
	void setTransparency(bool hasTrans);
	// note: top is mapchunk above -> mapchunk with lower y
	void setNeighbors(MapChunk* left, MapChunk* right, MapChunk* top, MapChunk* bottom);
private:
	// position in chunk + offset
	CubeBase* getCube(const Point3S& p, int x, int y, int z);
	CubeBase* getCube(const Point3S& p);
private:
	static const Point3S m_dim;
	std::vector<std::unique_ptr<CubeBase>> m_cubes;

	// GPU stuff
	bool m_hasChanged = true;
	bool m_hasTransparent = false;
	MapChunk* m_left = nullptr;
	MapChunk* m_right = nullptr;
	MapChunk* m_top = nullptr;
	MapChunk* m_bottom = nullptr;

	// pretend that this is a float because opengl would convert it otherwise...
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
	Mutex m_muCubes;
};
