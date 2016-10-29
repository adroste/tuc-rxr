#pragma once
// Map for Client only TODO server optimization
#include "Cube.h"
#include "../../Utility/Point3.h"
#include "../../Utility/Mutex.h"
#include "../../Framework/OpenGL/VolumeTextureMap.h"
#include "CubeBase.h"
#include "../Client/MapChunk.h"
#include "../Client/CubeMaterial.h"

class Map : public IDrawable
{
public:
	struct MaterialInfo
	{
		CubeMaterial m;
		size_t id;
	};
public:
	Map(Point3S dim);
	virtual ~Map();

	void setCube(Point3S pos, std::unique_ptr<CubeBase> c);
	void destroyBlock(const Point3S& pos);

	virtual void draw(Drawing& draw) override;
	void setDim(Point3S dim);

	Point3S getDim() const;
	std::vector<std::pair<CubeDesc, Point3S>> getCubeInfos();

	std::shared_ptr<MaterialInfo> addMaterial(const CubeMaterial& m);
private:
	std::vector<MapChunk> m_chunks;
	Point3S m_dim;
};
