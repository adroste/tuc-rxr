#pragma once
#include "UINumUpDownEnum.h"

class UINumUpDownLightSource : public UINumUpDownEnum<UniformBlockLight::LightSource::Type>
{
public:

	UINumUpDownLightSource(Font& font, UniformBlockLight::LightSource::Type start)
		: UINumUpDownEnum<UniformBlockLight::LightSource::Type>(font, start, UniformBlockLight::LightSource::Type::Directional, UniformBlockLight::LightSource::Type::PointLight)
	{
		UINumUpDown<UniformBlockLight::LightSource::Type>::setValue(start);
	}

protected:
	virtual std::string numToSting(UniformBlockLight::LightSource::Type n) const override
	{
		switch (n)
		{
		case UniformBlockLight::LightSource::Directional: return "directional";
		case UniformBlockLight::LightSource::PointLight: return "point";
		}
		return "ERROR";
	}

	virtual UniformBlockLight::LightSource::Type stringToNum(const std::string& s) const override
	{
		if (s == "directional")
			return UniformBlockLight::LightSource::Directional;
		else if (s == "point")
			return UniformBlockLight::LightSource::PointLight;

		return UniformBlockLight::LightSource::Directional;
	}
};
