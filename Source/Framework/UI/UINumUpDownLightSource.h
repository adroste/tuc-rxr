#pragma once
#include "UINumUpDownEnum.h"

class UINumUpDownLightSource : public UINumUpDownEnum<LightType>
{
public:

	UINumUpDownLightSource(Font& font, LightType start)
		: UINumUpDownEnum<LightType>(font, start, LightType::Directional, LightType::PointLight)
	{
		UINumUpDown<LightType>::setValue(start);
	}

protected:
	virtual std::string numToSting(LightType n) const override
	{
		return LightTypeToString(n);
	}

	virtual LightType stringToNum(const std::string& s) const override
	{
		auto t = LightTypeFromString(s);
		return t == LightType::Unknown ? LightType::Directional : t;
	}
};
