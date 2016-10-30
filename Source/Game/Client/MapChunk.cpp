#include "MapChunk.h"
#include <bitset>

MapChunk::MapChunk(Point3S dim)
	: m_dim(dim)
{
	assert(dim.height <= 256);
	assert(dim.width == 16);
	assert(dim.depth <= 16);

	// initialize cubes
	auto s = dim.size();
	m_cubes.reserve(s);
	for (size_t i = 0; i < s; i++)
		m_cubes.push_back(std::unique_ptr<CubeBase>(nullptr));
}

MapChunk::~MapChunk()
{
}

void MapChunk::draw(Drawing& draw, Mesh& cube)
{
	updateGpuArray(); // TODO move this function somewhere else?
	if (m_iArray.getDataCount() == 0)
		return;
	// create array or reupload if data changed
	m_iArray.flush();

	// draw instanced
	m_iArray.bind(2);

	cube.drawInstanced(m_iArray.getDataCount(), m_iArray);
}

void MapChunk::setCube(Point3S pos, std::unique_ptr<CubeBase> c)
{
	auto index = m_dim.calcIndex(pos);
	m_cubes[index] = move(c);
	m_hasChanged = true;
}

void MapChunk::updateGpuArray()
{
	if (!m_hasChanged)
		return;

	std::vector<unsigned int> gpuArray;
	gpuArray.reserve(m_cubes.size());

	size_t idx = 0;
	for (const auto& pCube : m_cubes)
	{
		if (pCube)
		{
			// add to gpuArray
			// set material index
			auto matIndex = pCube->getMaterialIndex();
			if (!matIndex)
				continue; // ignore block (not standart drawn)

			unsigned int v = 0;
			v |= (matIndex & 0xFFFF) << 16;
			assert((matIndex & ~size_t(0xFFFF)) == 0);

			// split index
			v |= idx & 0xFFFF;
			assert((idx & 0xFFFF0000) == 0);

			gpuArray.push_back(v);
		}

		idx++;
	}

	m_iArray.setData(move(gpuArray));
}

void MapChunk::resize(size_t height, size_t depth)
{
	assert(height <= 256);
	assert(depth <= 16);

	if (height == m_dim.height && depth == m_dim.depth)
		return;

	std::vector<std::unique_ptr<CubeBase>> newCubes;
	newCubes.reserve(height * depth * m_dim.width);
	Point3S newDim = Point3S(m_dim.width, height, depth);

	// transfer old cubes
	for (size_t z = 0; z < depth; z++)
	{
		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < m_dim.width; x++)
			{
				if(y < m_dim.height && z < m_dim.depth)
				{
					// transfer block
					newCubes.push_back(move(m_cubes[m_dim.calcIndex({ x, y, z })]));
				}
				else newCubes.push_back(std::unique_ptr<CubeBase>(nullptr));
			}
		}
	}

	m_dim = newDim;
	m_cubes = move(newCubes);
}
