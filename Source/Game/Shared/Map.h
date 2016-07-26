#pragma once
#include "Cube.h"
#include "../../Utility/Point3.h"
#ifdef _CLIENT
#include "../../Framework/OpenGL/VolumeTextureMap.h"
#endif

class Map : public IDrawable
{
public:
	Map(Point3S dim);
	virtual ~Map();

	void setCube(Cube* cube, bool overwrite = false);

#ifdef _CLIENT
	virtual void draw(Drawing& draw) override;
#endif // _CLIENT

	Point3S getDim() const;

private:
	size_t getIndex(Point3S pos) const;

private:
	Cube** m_ppCubes = nullptr;
	Point3S m_dim;

#ifdef _CLIENT
	std::unique_ptr<VolumeTextureMap> m_pTextureMap;
	bool m_texCreated = false;
#endif // _CLIENT
};
