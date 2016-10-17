#pragma once
#include "UINumUpDownEnum.h"

class UINumUpDownLightSource : public UINumUpDownEnum<LightSource::Type>
{
public:

	UINumUpDownLightSource(Font& font, LightSource::Type start)
		: UINumUpDownEnum<LightSource::Type>(font, start, LightSource::Type::Directional, LightSource::Type::PointLight)
	{
		UINumUpDown<LightSource::Type>::setValue(start);
	}

protected:
	virtual std::string numToSting(LightSource::Type n) const override
	{
		switch (n)
		{
		case LightSource::Directional: return "directional";
		case LightSource::PointLight: return "point";
		}
		return "ERROR";
	}

	virtual LightSource::Type stringToNum(const std::string& s) const override
	{
		if (s == "directional")
			return LightSource::Directional;
		else if (s == "point")
			return LightSource::PointLight;

		return LightSource::Directional;
	}
};
