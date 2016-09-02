#pragma once
#include "UINumUpDownEnum.h"

class UINumUpDownBlockType : public UINumUpDownEnum<CubeDesc::BlockType>
{
public:
	UINumUpDownBlockType(Font& font, CubeDesc::BlockType start)
		: UINumUpDownEnum<CubeDesc::BlockType>(font, start, CubeDesc::BlockType::Immortal, CubeDesc::BlockType::Liquid)
	{
		UINumUpDown<CubeDesc::BlockType>::setValue(start);
	}

protected:
	virtual std::string numToSting(CubeDesc::BlockType n) const override
	{
		switch (n)
		{
		case CubeDesc::Immortal: return "Immortal";
		case CubeDesc::Transparent: return "Transparent";
		case CubeDesc::Solid: return "Solid";
		case CubeDesc::Liquid: return "Liquid";
		}
		return "ERROR";
	}
	virtual CubeDesc::BlockType stringToNum(const std::string& s) const override
	{
		if (s == "Immortal")
			return CubeDesc::Immortal;
		if (s == "Transparent")
			return CubeDesc::Transparent;
		if (s == "Solid")
			return CubeDesc::Solid;
		if (s == "Liquid")
			return CubeDesc::Liquid;
		return CubeDesc::Immortal;
	}
};