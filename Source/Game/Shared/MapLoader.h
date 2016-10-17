#pragma once
#include <string>
#include "../../Utility/Point3.h"
#include "CubeDesc.h"
#include <vector>
#include "../../xml/tinyxml2.h"
#include "LightSource.h"
#include "../../Framework/Color.h"

class MapLoader
{
public:
	MapLoader(const std::string& filename);
	bool isOpen() const;

	// filename without .xml extension
	static void save(const std::string& filename, const Point3S& dim, std::vector<std::pair<CubeDesc, Point3S>> cubes,
		const std::vector<LightSource>& lights);
	static void parseCubeDescToXML(tinyxml2::XMLPrinter& p, const CubeDesc& d, size_t id);
	static bool parseXMLToCubeDesc(tinyxml2::XMLNode* node, CubeDesc& c, size_t* dstID = nullptr);
	static void parseLightToXML(tinyxml2::XMLPrinter& p, const LightSource& l);
	static bool parseXMLToLight(tinyxml2::XMLNode* node, LightSource& l);

	const std::vector<std::pair<CubeDesc, Point3S>>& getCubes() const;
	const Point3S& getDim() const;
	const std::vector<LightSource>& getLights() const;
	const Color& getAmbient() const;
private:
	static std::string colToString(uint32_t c);
	static std::string colToString(const glm::vec3& v);
	static uint32_t strToColor(const char* s);
	static float getFloat(const char* s);
	static std::string vecToString(const glm::vec3& v);
	static glm::vec3 strToVec3(const char* s);

	static const int s_Version = 1;
private:
	bool m_isValid = false;
	std::vector<std::pair<CubeDesc, Point3S>> m_cubes;
	std::vector<LightSource> m_lights;
	Color m_ambient;
	Point3S m_dim;
};
