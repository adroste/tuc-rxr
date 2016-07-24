#pragma once
#include "Cube.h"

class Map
{
public:
	Map(size_t width, size_t height, size_t depth = 4);
	virtual ~Map();

	void setCube(size_t x, size_t y, size_t z, Cube* cube);

#ifdef _CLIENT
	void draw(Drawing& draw);
#endif // _CLIENT

	size_t getWidth() const;
	size_t getHeight() const;
	size_t getDepth() const;

private:
	size_t getIndex(size_t x, size_t y, size_t z) const;

private:
	Cube** m_ppCubes = nullptr;
	size_t m_width;
	size_t m_height;
	size_t m_depth;
};