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

std::string BucketLoader::colToString(uint32_t c)
{
	char buffer[16];
	// print colors like: 0xFFFF0012
	sprintf(buffer, "0x%08x", c);
	return std::string(buffer);
}
