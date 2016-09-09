#pragma once
#include "UINumUpDownEnum.h"

class UINumUpDownBlockType : public UINumUpDownEnum<BlockType>
{
public:
	UINumUpDownBlockType(Font& font, BlockType start)
		: UINumUpDownEnum<BlockType>(font, start, BlockType::Immortal, BlockType::Liquid)
	{
		UINumUpDown<BlockType>::setValue(start);
	}

protected:
	virtual std::string numToSting(BlockType n) const override
	{
		auto s = BlockTypeToString(n);
		if (s.length())
			return s;
		else return "ERROR";
	}
	virtual BlockType stringToNum(const std::string& s) const override
	{
		return BlockTypeFromString(s);
	}
};