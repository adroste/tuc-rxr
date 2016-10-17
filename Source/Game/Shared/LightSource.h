#pragma once
#include "../../glm/detail/type_vec3.hpp"

struct LightSource
{
	enum Type
	{
		Directional,
		PointLight
	} type;
	glm::vec3 color;
	glm::vec3 origin;
	float attenuation;
};
