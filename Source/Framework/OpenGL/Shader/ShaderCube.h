#pragma once
#include "Shader.h"

class ShaderCube : public Shader, public Shader::ILoadable
{
public:
	virtual ~ShaderCube() {}
	void load() override;
};
