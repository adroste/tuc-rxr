#include "MaterialLoader.h"
#include "../../xml/tinyxml2.h"
#include "../../System/Log.h"
#include "CubeDesc.h"
#include <cassert>
#include "../../Framework/Color.h"
#include <vector>
#include "../../Utility/Point3.h"
#include "parser.h"

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
		node = node->NextSibling();
	}


	m_loaded = true;
}

bool MaterialLoader::isOpen() const
{
	return m_loaded;
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
		c.diffuse = parser::strToColor(attr);
	else return false;

	if ((attr = elm->Attribute("specular")))
		c.spec = parser::strToColor(attr);
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

void MaterialLoader::save(const std::string& filename, const std::vector<CubeDesc>& descs, bool indexed)
{
	FILE* pFile = fopen(filename.c_str(), "wb");
	if (!pFile)
		return;

	tinyxml2::XMLPrinter px(pFile);

	size_t index = 0;
	for (const auto& e : descs)
	{
		if (indexed) index++;
		writeCubeDesc(px, e, index);
	}

	fclose(pFile);
	pFile = nullptr;
}

std::map<size_t, CubeDesc> MaterialLoader::getMappedDesc() const
{
	// multimap to map
	std::map<size_t, CubeDesc> m(m_descs.begin(),m_descs.end());

	return m;
}

std::vector<CubeDesc> MaterialLoader::getDesc() const
{
	std::vector<CubeDesc> d;
	d.reserve(m_descs.size());
	for(const auto& e : m_descs)
		d.push_back(e.second);

	return d;
}

void MaterialLoader::writeCubeDesc(tinyxml2::XMLPrinter& p, const CubeDesc& c, size_t id)
{
	p.OpenElement("cube");

	if (id != 0)
		p.PushAttribute("id", std::to_string(id).c_str());

	p.PushAttribute("diffuse", parser::colToString(c.diffuse).c_str());
	p.PushAttribute("specular", parser::colToString(c.spec).c_str());
	p.PushAttribute("gloss", std::to_string(c.gloss).c_str());
	p.PushAttribute("shader", CubeShaderToString(c.shader).c_str());
	p.PushAttribute("blockType", BlockTypeToString(BlockType(c.blockType)).c_str());
	p.PushAttribute("gravity", (c.blockFlags & CubeDesc::Gravity) != 0);
	p.PushAttribute("HP", c.blockHP);

	p.CloseElement();
}