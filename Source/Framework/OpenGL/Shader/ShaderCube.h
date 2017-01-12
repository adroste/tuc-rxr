#pragma once
#include "Shader.h"
#include "ShaderCubeUniforms.h"

class ShaderCube : public ShaderCubeUniforms, public Shader::ILoadable
{
public:
	virtual ~ShaderCube() {}
	void load() override;
};
