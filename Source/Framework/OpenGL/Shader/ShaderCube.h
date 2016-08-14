#pragma once
#include "Shader.h"

class ShaderCube : public Shader, public Shader::Loadable
{
public:
	virtual ~ShaderCube() {}
	void load() override;
};
