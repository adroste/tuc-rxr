#include "MaterialLoader.h"
#include "../../xml/tinyxml2.h"
#include "../../System/Log.h"
#include "CubeDesc.h"
#include <cassert>
#include "../../Framework/Color.h"
#include <vector>
#include "../../Utility/Point3.h"

MaterialLoader::MaterialLoader(const std::string & filename)
{
	// open xml
	tinyxml2::XMLDocument doc;
	auto err = doc.LoadFile(filename.c_str());
	if (err != 0)
		return;

	// load xml document
	auto node = doc.FirstChild();
	while (node != nullptr)
	{
		if (node->Value() == std::string("cube"))
		{
			size_t id = 0;
			CubeDesc c;
			if(parseCubeDesc(node, c, &id))
			{
				m_descs.insert(std::pair<size_t,CubeDesc>(id,c));
			}
		}
		else Log::warning(std::string("unknown node type ") + node->Value() + " in " + filename);
	}


	m_loaded = true;
}

bool MaterialLoader::isOpen() const
{
	return true;
}

bool MaterialLoader::parseCubeDesc(tinyxml2::XMLNode* node, CubeDesc& c, size_t* dstID)
{
	assert(node->Value() == std::string("cube"));

	memset(&c, 0, sizeof(c));
	auto elm = node->ToElement();
	const char* attr = nullptr;
	// read attributes
	if ((attr = elm->Attribute("id")))
		*dstID = atoi(attr);

	if ((attr = elm->Attribute("diffuse")))
		c.diffuse = strToColor(attr);
	else return false;

	if ((attr = elm->Attribute("specular")))
		c.spec = strToColor(attr);
	else return false;

	if ((attr = elm->Attribute("gloss")))
		c.gloss = float(atof(attr));
	else return false;

	if ((attr = elm->Attribute("shader")))
		c.shader = CubeShaderFromString(attr);
	else return false;

	if ((attr = elm->Attribute("blockType")))
		c.blockType = uint8_t(BlockTypeFromString(attr));
	else return false;

	if ((attr = elm->Attribute("HP")))
		c.blockHP = atoi(attr);
	else return false;

	if ((attr = elm->Attribute("gravity")))
		if (attr == std::string("true"))
			c.blockFlags |= CubeDesc::Gravity;

	return true;
}

void MaterialLoader::save(const std::string& filename, const std::vector<std::pair<size_t, CubeDesc>>& descs)
{
}

std::string MaterialLoader::colToString(uint32_t c)
{
	char buffer[16];
	sprintf(buffer, "0x%08x", c);
	return std::string(buffer);
}

std::string MaterialLoader::colToString(const glm::vec3& v)
{
	Color c = Color(v.r, v.g, v.b);
	return colToString(c.toDWORD());
}

uint32_t MaterialLoader::strToColor(const char* s)
{
	auto str = std::string(s);
	if (str.size() > 2)
	{
		str = str.substr(2, str.length() - 2);
		try
		{
			auto i = uint32_t(int32_t(std::strtoll(s, nullptr, 16)));
			return i;
		}
		catch (const std::exception&) {}
	}
	return 0;
}

float MaterialLoader::getFloat(const char* s)
{
	return float(atof(s));
}

std::string MaterialLoader::vecToString(const glm::vec3& v)
{
	return std::to_string(v.r) + " " + std::to_string(v.g) + " " + std::to_string(v.b);
}

glm::vec3 MaterialLoader::strToVec3(const char* s)
{
	glm::vec3 v;
	sscanf(s, "%f %f %f", &v.r, &v.g, &v.b);
	return v;
}