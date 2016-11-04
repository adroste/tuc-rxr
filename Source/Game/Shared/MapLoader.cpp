#include "MapLoader.h"
#include "../../xml/tinyxml2.h"
#include <set>
#include "../../Utility/FileReader.h"
#include "../../Framework/Color.h"
#include "parser.h"
#include "MaterialLoader.h"
#include "ChunkLoader.h"

template<class T>
class IndexedSet
{
public:
	// returns index of added material
	size_t add(const T& t)
	{
		size_t i = 0;
		for (const auto& e : m_data)
		{
			if (e == t)
				return i + 1;
			++i;
		}
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
	tinyxml2::XMLDocument doc;
	auto err = doc.LoadFile((filename + ".cmap").c_str());
	if (err != 0) return;
	
	std::string materialFilename;
	std::vector<std::string> chunkFiles;

	bool hasMapinfo = false;

	auto node = doc.FirstChild();
	while(node != nullptr)
	{
		if(node->Value() == std::string("map"))
		{
			assert(!hasMapinfo);
			hasMapinfo = true;
			{
				auto elm = node->ToElement();
				const char* attr = nullptr;

				if ((attr = elm->Attribute("width")))
					m_info.nChunks.x = parser::getInt(attr);
				else return;

				if ((attr = elm->Attribute("height")))
					m_info.nChunks.y = parser::getInt(attr);
				else return;

				if ((attr = elm->Attribute("material")))
					materialFilename = attr;
				else return;
			}
			chunkFiles.clear();
			chunkFiles.assign(m_info.nChunks.x * m_info.nChunks.y, std::string());

			// read chunks
			auto map = node->FirstChild();
			while(map != nullptr)
			{
				if(map->Value() == std::string("chunk"))
				{
					auto elm = map->ToElement();
					const char* attr = nullptr;

					Point3S chpos;
					if ((attr = elm->Attribute("x")))
						chpos.x = parser::getInt(attr);
					else return;

					if ((attr = elm->Attribute("y")))
						chpos.y = parser::getInt(attr);
					else return;

					std::string chunkFilename;
					if ((attr = elm->Attribute("file")))
						chunkFilename = attr;
					else return;

					if(chpos.x < m_info.nChunks.x || chpos.y < m_info.nChunks.y)
					{
						chunkFiles[chpos.y * m_info.nChunks.x + chpos.x] = chunkFilename;
					}
				}
				map = map->NextSibling();
			}
		}
		else if(node->Value() == std::string("light"))
		{
			LightSource l;
			if(parseLight(node, l))
			{
				if (l.type == LightType::Ambient)
					m_info.ambient = Color(l.color.r, l.color.g, l.color.b);
				else
					m_info.lights.push_back(l);
			}
		}
		node = node->NextSibling();
	}
	doc.Clear();

	if (!hasMapinfo) return;

	// load chunks
	std::string path;
	tool::splitFilenameToPathFile(&path, nullptr, filename);
	MaterialLoader ml(path + materialFilename);
	if (!ml.isOpen())
		return;

	m_info.chunkCubes.assign(m_info.nChunks.x * m_info.nChunks.y, MapInfo::ChunkCubes());

	size_t idx = 0;
	for(const auto& fname : chunkFiles)
	{
		ChunkLoader cl(path + fname, ml.getMappedDesc());
		if(cl.isOpen())
		{
			m_info.chunkCubes[idx] = cl.getCubes();
		}
		idx++;
	}

	m_isValid = true;
}

bool MapLoader::isOpen() const
{
	return m_isValid;
}

const MapLoader::MapInfo& MapLoader::getInfo() const
{
	return m_info;
}

void MapLoader::save(const std::string& filename, const MapInfo& i)
{
	// save map info
	FILE* pFile = fopen((filename + ".cmap").c_str(), "wb");
	tinyxml2::XMLPrinter p(pFile);

	std::string path;
	std::string file;
	tool::splitFilenameToPathFile(&path, &file, filename);

	p.OpenElement("map");
	{
		p.PushAttribute("width", i.nChunks.x);
		p.PushAttribute("height", i.nChunks.y);
		p.PushAttribute("material", getMaterialFile(path,file).c_str());
		// add chunks
		for(size_t y = 0; y < i.nChunks.y; y++)
		for(size_t x = 0; x < i.nChunks.x; x++)
		{
			p.OpenElement("chunk");

			p.PushAttribute("x", x);
			p.PushAttribute("y", y);
			p.PushAttribute("file", getChunkFile(path, file, x, y).c_str());

			p.CloseElement("chunk");
		}
	}
	p.CloseElement();

	// write lights
	{
		LightSource l;
		l.type = LightType::Ambient;
		l.color = i.ambient.toVec3();
		writeLight(p, l);
	}
	for(const auto& l : i.lights)
	{
		writeLight(p, l);
	}

	// write assets

	fclose(pFile);
	pFile = nullptr;

	// create material and chunk files
	IndexedSet<CubeDesc> materials;

	// create chunks
	size_t cindex = 0;
	for(const auto& chunk : i.chunkCubes)
	{
		std::vector<std::pair<Point3S, size_t>> indices;
		for(const auto& c : chunk)
		{
			size_t idx = materials.add(c.second);
			indices.push_back(std::make_pair(c.first, idx));
		}
		PointS pos = i.nChunks.fromIndex(cindex);
		ChunkLoader::save(getChunkFile(path, file, pos.x, pos.y), indices);
		cindex++;
	}

	// create material
	MaterialLoader::save(getMaterialFile(path, file), materials.get(), true);
}

void MapLoader::writeLight(tinyxml2::XMLPrinter& p, const LightSource& l)
{
	p.OpenElement("light");

	p.PushAttribute("type", LightTypeToString(l.type).c_str());
	p.PushAttribute("color", parser::colToString(l.color).c_str());
	switch (l.type)
	{
	case LightType::Directional:
		p.PushAttribute("direction", parser::vecToString(l.origin).c_str());
		break;
	case LightType::PointLight:
		p.PushAttribute("origin", parser::vecToString(l.origin).c_str());
		p.PushAttribute("attenuation", std::to_string(l.attenuation).c_str());
		break;
	case LightType::Ambient: break;
	default: break;
	}

	p.CloseElement();
}

bool MapLoader::parseLight(tinyxml2::XMLNode* node, LightSource& l)
{
	assert(node->Value() == std::string("light"));
	memset(&l, 0, sizeof(l));
	auto elm = node->ToElement();
	const char* attr = nullptr;

	if ((attr = elm->Attribute("type")))
		l.type = LightTypeFromString(attr);
	else return false;

	if ((attr = elm->Attribute("color")))
		l.color = Color(parser::strToColor(attr)).toVec3();
	else return false;

	switch (l.type)
	{
	case LightType::Directional:
		if ((attr = elm->Attribute("direction")))
			l.origin = parser::strToVec3(attr);
		else return false;
		break;
	case LightType::PointLight:
		if ((attr = elm->Attribute("origin")))
			l.origin = parser::strToVec3(attr);
		else return false;
		if ((attr = elm->Attribute("attenuation")))
			l.attenuation = parser::getFloat(attr);
		else return false;
		break;
	default: break;
	}

	return true;
}

std::string MapLoader::getMaterialFile(const std::string& path, const std::string& file)
{
	return path + file + "_material.cd";
}

std::string MapLoader::getChunkFile(const std::string& path, const std::string& file, size_t x, size_t y)
{
	return path + file + "_chunk_" + std::to_string(x) + "_" + std::to_string(y) + ".bin";
}