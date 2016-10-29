#pragma once
#include "../../glm/detail/type_vec4.hpp"

struct CubeMaterial
{
	glm::vec4 diffuse;
	glm::vec4 specular; // specular.w = gloss
};
