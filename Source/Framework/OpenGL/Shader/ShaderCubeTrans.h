#pragma once
#include "Shader.h"

class ShaderCubeTrans: public ShaderCubeUniforms, public Shader::ILoadable
{
public:
	ShaderCubeTrans() = default;
	virtual ~ShaderCubeTrans() override = default;

	void load() override
	{
		Shader::load("data/Shader/cube_trans");
	}
};