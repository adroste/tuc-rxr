#pragma once
#include <string>
#include "../../Utility/Point3.h"
#include "CubeDesc.h"
#include <vector>
#include "../../xml/tinyxml2.h"

class MapLoader
{
public:
	MapLoader(const std::string& filename);
	bool isOpen() const;

	// filename without .xml extension
	static void save(const std::string& filename, const Point3S& dim, std::vector<std::pair<CubeDesc, Point3S>> cubes);
	static void parseCubeDescToXML(tinyxml2::XMLPrinter& p, const CubeDesc& d, size_t id);
	static bool parseXMLToCubeDesc(tinyxml2::XMLNode* node, CubeDesc& c, size_t* dstID = nullptr);
private:
	static std::string colToString(uint32_t c);
	static uint32_t strToColor(const char* s);
	static float getFloat(const char* s);

	static const int s_Version = 1;
};
