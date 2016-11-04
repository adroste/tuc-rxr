#pragma once
#include <string>
#include "../../xml/tinyxml2.h"
#include "CubeDesc.h"
#include "../../glm/detail/type_vec3.hpp"
#include <vector>

class MaterialLoader
{
public:
	MaterialLoader(const std::string& filename);
	bool isOpen() const;
	static void save(const std::string& filename, const std::vector<std::pair<size_t, CubeDesc>>& descs);
	static void save(const std::string& filename, const std::vector<CubeDesc>& descs);
	std::map<size_t, CubeDesc> getMappedDesc() const;
	std::vector<CubeDesc> getDesc() const;
private:
	static void writeCubeDesc(tinyxml2::XMLPrinter& p, const CubeDesc& c, size_t id);
	static bool parseCubeDesc(tinyxml2::XMLNode* node, CubeDesc& c, size_t* dstID);
	static std::string colToString(uint32_t c);
	static std::string colToString(const glm::vec3& v);
	static uint32_t strToColor(const char* s);
	static float getFloat(const char* s);
	static std::string vecToString(const glm::vec3& v);
	static glm::vec3 strToVec3(const char* s);
private:
	bool m_loaded = false;
	std::multimap<size_t, CubeDesc> m_descs;
};
