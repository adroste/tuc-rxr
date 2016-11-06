#pragma once
#include <string>
#include "../../Utility/Point3.h"
#include "CubeDesc.h"
#include <vector>
#include "../../xml/tinyxml2.h"
#include "LightSource.h"
#include "../../Framework/Color.h"
#include "../../Utility/Point.h"

enum class AssetAnimation
{
	None,
	Wind,
	SIZE
};

ENUM_CONVERT_FUNC(AssetAnimation);

class MapLoader
{
public:
	using ChunkCubes = std::vector<std::pair<Point3S, CubeDesc>>;
	struct AssetInfo
	{
		ChunkCubes geometry;
		struct Item
		{
			glm::vec3 pos;
			float scale;
			float theta;
			float phi;
		};
		std::vector<Item> instances;
		AssetAnimation animation;
	};
	struct MapInfo
	{
		// chunk data
		PointS nChunks; // how mny chunks in x and y?
		std::vector<ChunkCubes> chunkCubes;

		// light
		std::vector<LightSource> lights;
		Color ambient;

		// assets
		std::vector<AssetInfo> assets;
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
	static std::string getAssetFile(const std::string& path, const std::string& file, size_t num);
	static std::string getAssetMaterialFile(const std::string& path, const std::string& file, size_t num);
	static std::string getAssetChunkFile(const std::string& path, const std::string& file, size_t num);
private:
	bool m_isValid = false;
	MapInfo m_info;
};
