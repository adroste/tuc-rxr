#pragma once
#include "Cube.h"
#include "../../Utility/Point3.h"

class Map
{
public:
	Map(size_t width, size_t height, size_t depth = 4);
	Map(Point3S dim);
	virtual ~Map();

	void setCube(Cube* cube, bool overwrite = false);

#ifdef _CLIENT
	void draw(Drawing& draw);
#endif // _CLIENT

	Point3S getDim() const;

private:
	size_t getIndex(Point3S pos) const;

private:
	Cube** m_ppCubes = nullptr;
	Point3S m_dim;
};
