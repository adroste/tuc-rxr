#include "BucketLoader.h"
#include "../../xml/tinyxml2.h"
#include <string>
#include "../Shared/MapLoader.h"

void BucketLoader::saveBucket(const std::string& filename, const std::vector<CubeDesc>& cubes)
{
	FILE* pFile = fopen(filename.c_str(), "w");
	if(!pFile)
		return;

	tinyxml2::XMLPrinter p(pFile);

	for(const auto& c : cubes)
	{
		MapLoader::parseCubeDescToXML(p, c, 0);
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
			
			if(MapLoader::parseXMLToCubeDesc(node,c,nullptr))
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
