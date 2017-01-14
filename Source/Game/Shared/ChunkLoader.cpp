#include "ChunkLoader.h"
#include "../../Utility/FileReader.h"
#include "../../System/Log.h"
#include "../../Utility/FileWriter.h"
#include "StaticChunk.h"

ChunkLoader::ChunkLoader(const std::string& filename, const std::map<size_t, CubeDesc>& material)
{
	FileReader r(filename);
	if (!r.isOpen()) return;

	try
	{
		if (r.readChar() != 'C') return;
		if (r.readChar() != 'N') return;
		if (r.readChar() != 'K') return;
		m_version = r.readInt();
		if (m_version != s_version) return;
		m_chunkDim = r.readByte();
		if (m_chunkDim != StaticChunk::SIZE) return;

		Point3S dim = Point3S(m_chunkDim,m_chunkDim,m_chunkDim);
		size_t len = m_chunkDim * m_chunkDim * m_chunkDim;
		for(size_t i = 0; i < len; i++)
		{
			size_t id = static_cast<size_t>(r.readShort());
			if(id != 0)
			{
				auto it = material.find(id);
				if(it != material.end())
				{
					m_cubes.push_back(std::make_pair(dim.fromIndex(i), it->second));
				}
				else
				{
					Log::warning("cannot find cube material for id " + std::to_string(id) + " in " + filename);
					// push pink cube
					m_cubes.push_back(std::make_pair(dim.fromIndex(i), CubeDesc::getUgly()));
				}
			}
		}
	}
	catch (const std::out_of_range&)
	{
		Log::warning("out of range cought in file: " + filename);
		return;
	}
	m_loaded = true;
}

bool ChunkLoader::isOpen() const
{
	return m_loaded;
}

void ChunkLoader::save(const std::string& filename, const std::vector<std::pair<Point3S, size_t>>& cubes)
{
	FileWriter w(filename);
	if (!w.isOpen()) return;

	w.writeChar('C');
	w.writeChar('N');
	w.writeChar('K');
	w.writeInt(s_version);
	w.writeByte(StaticChunk::SIZE);
	
	Point3S dim = Point3S(StaticChunk::SIZE, StaticChunk::SIZE, StaticChunk::SIZE);
	std::vector<unsigned short> c;
	c.assign(dim.size(), 0);

	for(const auto& b : cubes)
	{
		c[dim.calcIndex(b.first)] = static_cast<unsigned short>(b.second);
	}

	w.writeArray(&c[0], c.size() * sizeof(short));
}

const std::vector<std::pair<Point3S, CubeDesc>>& ChunkLoader::getCubes() const
{
	return m_cubes;
}
