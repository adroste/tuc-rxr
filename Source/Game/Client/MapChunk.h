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
	static const size_t SIZE = 32;
public:
	MapChunk();
	~MapChunk();
	MOVE_ONLY(MapChunk);

	void draw(Drawing& draw, Mesh& cube);
	void setCube(Point3S pos, std::unique_ptr<CubeBase> c);
	void updateGpuArray();
private:
	static const Point3S m_dim;
	std::vector<std::unique_ptr<CubeBase>> m_cubes;

	// GPU stuff
	bool m_hasChanged = true;
	// pretend that this is a float because opengl would convert it otherwise...
	InstacingArray<glm::ivec3, 3, GL_FLOAT> m_iArray;
	/*
	 * x:	0-15 chunk position
	 *		16-31 diffuse r+g
	 *	
	 *	y:	0-7 diffuse b
	 *		8-31 specular rgb
	 *	
	 *	z:	0-15 gloss (int)
	 *
	 */
};
