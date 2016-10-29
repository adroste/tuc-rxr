#include "ShaderMaterialHolder.h"

std::shared_ptr<const MaterialInfo> ShaderMaterialHolder::getMaterialInfo(const CubeMaterial & cm)
{
	// is material already used?
	for(const auto& m : m_mats)
	{
		if(m->m == cm)
		{
			return m;
		}
	}
	// create new material
	MaterialInfo i;
	i.m = cm;
	i.id = m_mats.size() + 1;
	auto ptr = std::shared_ptr<MaterialInfo>(new MaterialInfo(i));
	m_mats.push_back(ptr);
	return ptr;
}

bool ShaderMaterialHolder::clearUnused()
{
	// unused materials?
	bool shared = true;
	for(const auto& m : m_mats)
		if(m.unique())
		{
			shared = false;
			break;
		}

	if (shared) // nothing is unique
		return false;

	decltype(m_mats) newMats;
	for(auto& m : m_mats)
	{
		if(!m.unique())
		{
			m->id = newMats.size() + 1;
			newMats.push_back(m);
		}
	}

	m_mats = move(newMats);

	return true;
}
