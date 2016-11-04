#pragma once
#include <string>
#include "../../Utility/Point3.h"
#include "CubeDesc.h"
#include <vector>
#include "../../xml/tinyxml2.h"
#include "LightSource.h"
#include "../../Framework/Color.h"
#include "../../Utility/Point.h"

class MapLoader
{
public:
	struct MapInfo
	{
		// chunk data
		PointS nChunks; // how mny chunks in x and y?
		using ChunkCubes = std::vector<std::pair<Point3S, CubeDesc>>;
		std::vector<ChunkCubes> chunkCubes;

		// light
		std::vector<LightSource> lights;
		Color ambient;

		// assets
	};
public:
	MapLoader(const std::string& filename);
	bool isOpen() const;
	const MapInfo& getInfo() const;
	static void save(const std::string& filename, const MapInfo& i);
private:
	static void writeLight(tinyxml2::XMLPrinter& p, const LightSource& l);
	static bool parseLight(tinyxml2::XMLNode* node, LightSource& l);
	static std::string getMaterialFile(const std::string& path, const std::string& file);
	static std::string getChunkFile(const std::string& path, const std::string& file, size_t x, size_t y);
private:
	bool m_isValid = false;
	MapInfo m_info;
};
