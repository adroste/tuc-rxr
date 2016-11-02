#pragma once
#include "../../Game/Shared/CubeDesc.h"
#include "UINumUpDownEnum.h"

class UINumUpDownCubeShader : public UINumUpDownEnum<CubeShader>
{
public:

	UINumUpDownCubeShader(Font& font, CubeShader start, CubeShader min, CubeShader max, CubeShader step)
		: UINumUpDownEnum<CubeShader>(font, start, min, max)
	{
		UINumUpDown<CubeShader>::setValue(start);
	}

protected:
	virtual std::string numToSting(CubeShader n) const override
	{
		auto s = CubeShaderToString(n);
		if (s.size() == 0)
			return "invalid";
		return s;
	}

	virtual CubeShader stringToNum(const std::string& s) const override
	{
		return CubeShaderFromString(s);
	}
};
