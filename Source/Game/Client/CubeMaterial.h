#pragma once
#include "../../glm/detail/type_vec4.hpp"
#include "../Shared/CubeDesc.h"
#include "../../Framework/Color.h"

struct CubeMaterial
{
	glm::vec4 diffuse;
	glm::vec4 specular; // specular.w = gloss
	bool operator==(const CubeMaterial& r) const
	{
		return diffuse == r.diffuse && specular == r.specular;
	}
	static CubeMaterial fromDesc(const CubeDesc& desc)
	{
		Color d = Color(desc.diffuse);
		Color s = Color(desc.spec);
		CubeMaterial m;
		m.diffuse.r = d.r;
		m.diffuse.g = d.g;
		m.diffuse.b = d.b;
		m.diffuse.a = 0.0f;

		m.specular.r = s.r;
		m.specular.g = s.g;
		m.specular.b = s.b;
		m.specular.a = desc.gloss;
		return m;
	}
};
