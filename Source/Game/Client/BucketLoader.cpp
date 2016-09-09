#include "BucketLoader.h"
#include "../../xml/tinyxml2.h"
#include <string>

void BucketLoader::saveBucket(const std::string& filename, const std::vector<CubeDesc>& cubes)
{
	FILE* pFile = fopen(filename.c_str(), "w");
	if(!pFile)
		return;

	tinyxml2::XMLPrinter p(pFile);

	for(const auto& c : cubes)
	{
		p.OpenElement("cube");

		p.PushAttribute("diffuse", colToString(c.diffuse).c_str());
		p.PushAttribute("specular", colToString(c.spec).c_str());
		p.PushAttribute("gloss", std::to_string(c.gloss).c_str());
		p.PushAttribute("shader", CubeShaderToString(c.shader).c_str());
		p.PushAttribute("blockType",BlockTypeToString(BlockType(c.blockType)).c_str());
		p.PushAttribute("gravity", (c.blockFlags & CubeDesc::Gravity) != 0);
		p.PushAttribute("HP", c.blockHP);

		p.CloseElement(); // cube
	}

	fclose(pFile);
	pFile = nullptr;
}

BucketLoader::BucketLoader(const std::string& filename)
{
	// TODO reason if invalid

	tinyxml2::XMLDocument doc;
	auto err = doc.LoadFile(filename.c_str());
	if (err != 0)
		return; // invalid :/

	auto node = doc.FirstChild();
	if (!node)
		return;

	// inspect nodes
	while(node != nullptr)
	{
		if(node->Value() == std::string("cube"))
		{
			CubeDesc c;
			memset(&c, 0, sizeof(c));
			auto elm = node->ToElement();
			const char* attr = nullptr;
			// read attributes
			if ((attr = elm->Attribute("diffuse")))
				c.diffuse = strToColor(attr);
			else continue;
			
			if ((attr = elm->Attribute("specular")))
				c.spec = strToColor(attr);
			else continue;

			if ((attr = elm->Attribute("gloss")))
				c.gloss = atof(attr);
			else continue;

			if ((attr = elm->Attribute("shader")))
				c.shader = CubeShaderFromString(attr);
			else continue;

			if ((attr = elm->Attribute("blockType")))
				c.blockType = uint8_t(BlockTypeFromString(attr));
			else continue;

			if ((attr = elm->Attribute("HP")))
				c.blockHP = atoi(attr);
			else continue;

			if ((attr = elm->Attribute("gravity")))
				if (attr == std::string("true"))
					c.blockFlags |= CubeDesc::Gravity;

			m_cubes.push_back(c);
		}
		node = node->NextSibling();
	}

	if (m_cubes.size())
		m_isValid = true;
}

bool BucketLoader::isOpen() const
{
	return m_isValid;
}

const std::vector<CubeDesc>& BucketLoader::getCubes() const
{
	return m_cubes;
}

std::string BucketLoader::colToString(uint32_t c)
{
	char buffer[16];
	// print colors like: 0xFFFF0012
	sprintf(buffer, "0x%08x", c);
	return std::string(buffer);
}

uint32_t BucketLoader::strToColor(const char* s)
{
	auto str = std::string(s);
	if(str.size() > 2)
	{
		str = str.substr(2, str.length() - 2);
		try
		{
			auto i = uint32_t( int32_t( std::strtoll(s, nullptr, 16) ) );
			return i;
		}
		catch (const std::exception&){}
	}
	return 0;
}

float BucketLoader::getFloat(const char* s)
{
	return atof(s);
}
