#pragma once
#include "Cube.h"
#include "../../Utility/Point3.h"
#include "../../Utility/Mutex.h"
#ifdef _CLIENT
#include "../../Framework/OpenGL/VolumeTextureMap.h"
#endif

class Map : public IDrawable
{
public:
	Map(Point3S dim);
	virtual ~Map();

	void setCube(Cube* cube, bool isLight = false, bool overwrite = false);
	void destroyBlock(const Point3S& pos);

#ifdef _CLIENT
	virtual void draw(Drawing& draw) override;
	void setDim(Point3S dim);
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
	Mutex m_muMap;
#endif // _CLIENT
};
