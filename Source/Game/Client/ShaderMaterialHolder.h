#pragma once
#include "CubeMaterial.h"
#include <vector>
#include <memory>

struct MaterialInfo
{
	CubeMaterial m;
	size_t id;
};

class ShaderMaterialHolder
{
public:
	std::shared_ptr<const MaterialInfo> getMaterialInfo(const CubeMaterial& cm);
	bool clearUnused();
private:
	std::vector<std::shared_ptr<MaterialInfo>> m_mats;
};
