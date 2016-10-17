#include "MapLoader.h"
#include "../../xml/tinyxml2.h"
#include <set>
#include "../../Utility/FileReader.h"
#include "../../Framework/Color.h"

template<class T>
class IndexedSet
{
public:
	// returns index of added material
	size_t add(const T& t, bool& added)
	{
		size_t i = 0;
		added = false;
		for (const auto& e : m_data)
		{
			if (e == t)
				return i + 1;
			++i;
		}
		// element was not in list
		added = true;
		m_data.push_back(t);
		return m_data.size();
	}
	const std::vector<T>& get() const
	{
		return m_data;
	}
private:
	std::vector<T> m_data;
};

MapLoader::MapLoader(const std::string & filename)
{
	// TODO reason
	std::vector<CubeDesc> materials;

	{
		tinyxml2::XMLDocument doc;
		auto err = doc.LoadFile((filename + ".xml").c_str());
		if (err != 0)
			return;

		// load xml document
		auto node = doc.FirstChild();
		while (node != nullptr)
		{
			if (node->Value() == std::string("cube"))
			{
				size_t id = 1;
				CubeDesc c;
				if (MapLoader::parseXMLToCubeDesc(node, c, &id))
				{
					// TODO improve
					assert(id != 0);
					while (int(materials.size()) < id) materials.push_back(CubeDesc());
					materials[id - 1] = c;
				}
			}
			else if(node->Value() == std::string("light"))
			{
				LightSource l;
				if(parseXMLToLight(node,l))
				{
					if (l.type == LightType::Ambient)
						m_ambient = Color(l.color.r, l.color.g, l.color.b);
					else
						m_lights.push_back(l);
				}
			}
			node = node->NextSibling();
		}
	}

	// load binary file
	// header

	FileReader r(filename + ".dat");
	
	if (!r.isOpen())return;
	
	try
	{
		if (!(r.readChar() == 'R' && r.readChar() == 'X' && r.readChar() == 'R'))
			return;

		auto version = r.readInt();
		if (version != s_Version) return;
		r.readStruct(&m_dim);

		auto len = m_dim.size();
		for(size_t i = 0; i < len; i++)
		{
			size_t id = static_cast<size_t>(r.readShort());
			if(id != 0)
			{
				// add block
				m_cubes.push_back(std::make_pair(materials[id - 1], m_dim.fromIndex(i)));
			}
		}
	}
	catch(const std::out_of_range&)
	{
		return;
	}
	m_isValid = true;
}

bool MapLoader::isOpen() const
{
	return m_isValid;
}

void MapLoader::save(const std::string& filename, const Point3S& dim, std::vector<std::pair<CubeDesc, Point3S>> cubes, const std::vector<LightSource>& lights)
{
	FILE* pXmlFile = fopen((filename + ".xml").c_str(), "wb");
	if (!pXmlFile) return;

	FILE* pBinFile = fopen((filename + ".dat").c_str(), "wb");
	if (!pBinFile) { fclose(pXmlFile); return; }

	// Binary File Header
	fwrite("RXR", 1, 3, pBinFile);
	// version
	fwrite(&s_Version, 1, sizeof(s_Version), pBinFile);
	fwrite(&dim, sizeof(dim), 1, pBinFile);

	tinyxml2::XMLPrinter px(pXmlFile);

	IndexedSet<CubeDesc> materials;

	std::vector<unsigned short> mapCubes;
	mapCubes.assign(dim.size(), 0);

	for(const auto& block : cubes)
	{
		bool addedMaterial = false;

		auto matIdx = materials.add(block.first, addedMaterial);
		if(addedMaterial)
		{
			parseCubeDescToXML(px, block.first, matIdx);
		}

		// add to map binary
		mapCubes[dim.calcIndex(block.second)] = short(matIdx);
	}

	// write map
	fwrite(&mapCubes[0], sizeof(unsigned short), mapCubes.size(), pBinFile);

	// write lights
	for(const auto& l : lights)
	{
		parseLightToXML(px, l);
	}

	fclose(pBinFile);
	pBinFile = nullptr;
	fclose(pXmlFile);
	pXmlFile = nullptr;
}

void MapLoader::parseCubeDescToXML(tinyxml2::XMLPrinter& p, const CubeDesc& c, size_t id)
{
	p.OpenElement("cube");

	p.PushAttribute("diffuse", colToString(c.diffuse).c_str());
	p.PushAttribute("specular", colToString(c.spec).c_str());
	p.PushAttribute("gloss", std::to_string(c.gloss).c_str());
	p.PushAttribute("shader", CubeShaderToString(c.shader).c_str());
	p.PushAttribute("blockType", BlockTypeToString(BlockType(c.blockType)).c_str());
	p.PushAttribute("gravity", (c.blockFlags & CubeDesc::Gravity) != 0);
	p.PushAttribute("HP", c.blockHP);

	if (id != 0)
		p.PushAttribute("id", std::to_string(id).c_str());

	p.CloseElement(); // cube
}

bool MapLoader::parseXMLToCubeDesc(tinyxml2::XMLNode* node, CubeDesc& c, size_t* dstID)
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

void MapLoader::parseLightToXML(tinyxml2::XMLPrinter& p, const LightSource& l)
{
	p.OpenElement("light");

	p.PushAttribute("type", LightTypeToString(l.type).c_str());
	p.PushAttribute("color", colToString(l.color).c_str());
	switch (l.type)
	{
	case LightType::Directional: 
		p.PushAttribute("direction", vecToString(l.origin).c_str());
		break;
	case LightType::PointLight:
		p.PushAttribute("origin", vecToString(l.origin).c_str());
		p.PushAttribute("attenuation", std::to_string(l.attenuation).c_str());
		break;
	case LightType::Ambient: break;
	default: break;
	}

	p.CloseElement();
}

bool MapLoader::parseXMLToLight(tinyxml2::XMLNode* node, LightSource& l)
{
	assert(node->Value() == std::string("light"));
	memset(&l, 0, sizeof(l));
	auto elm = node->ToElement();
	const char* attr = nullptr;

	if ((attr = elm->Attribute("type")))
		l.type = LightTypeFromString(attr);
	else return false;

	if ((attr = elm->Attribute("color")))
		l.color = Color(strToColor(attr)).toVec3();
	else return false;

	switch (l.type)
	{
	case LightType::Directional:
		if ((attr = elm->Attribute("direction")))
			l.origin = strToVec3(attr);
		else return false;
		break;
	case LightType::PointLight:
		if ((attr = elm->Attribute("origin")))
			l.origin = strToVec3(attr);
		else return false;
		if ((attr = elm->Attribute("attenuation")))
			l.attenuation = getFloat(attr);
		else return false;
		break;
	default: break;
	}

	return true;
}

std::string MapLoader::colToString(uint32_t c)
{
	char buffer[16];
	sprintf(buffer, "0x%08x", c);
	return std::string(buffer);
}

std::string MapLoader::colToString(const glm::vec3& v)
{
	Color c = Color(v.r, v.g, v.b);
	return colToString(c.toDWORD());
}

uint32_t MapLoader::strToColor(const char* s)
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

float MapLoader::getFloat(const char* s)
{
	return float(atof(s));
}

std::string MapLoader::vecToString(const glm::vec3& v)
{
	return std::to_string(v.r) + " " + std::to_string(v.g) + " " + std::to_string(v.b);
}

glm::vec3 MapLoader::strToVec3(const char* s)
{
	glm::vec3 v;
	sscanf(s, "%f %f %f", &v.r, &v.g, &v.b);
	return v;
}

const std::vector<std::pair<CubeDesc, Point3S>>& MapLoader::getCubes() const
{
	return m_cubes;
}

const Point3S& MapLoader::getDim() const
{
	return m_dim;
}

const std::vector<LightSource>& MapLoader::getLights() const
{
	return m_lights;
}

const Color& MapLoader::getAmbient() const
{
	return m_ambient;
}
