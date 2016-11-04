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
	static void save(const std::string& filename, const std::vector<CubeDesc>& descs, bool indexed = false);
	std::map<size_t, CubeDesc> getMappedDesc() const;
	std::vector<CubeDesc> getDesc() const;
private:
	static void writeCubeDesc(tinyxml2::XMLPrinter& p, const CubeDesc& c, size_t id);
	static bool parseCubeDesc(tinyxml2::XMLNode* node, CubeDesc& c, size_t* dstID);
private:
	bool m_loaded = false;
	std::multimap<size_t, CubeDesc> m_descs;
};
