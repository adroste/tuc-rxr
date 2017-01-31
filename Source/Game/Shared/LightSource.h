#pragma once
#include <glm/glm.hpp>
#include "../../Utility/EnumConverter.h"

enum class LightType
{
	Directional,
	PointLight,

	Ambient,
	Unknown
};

ENUM_CONVERT_FUNC(LightType);

struct LightSource
{
	LightType type;
	glm::vec3 color;
	glm::vec3 origin;
	float attenuation;
};
