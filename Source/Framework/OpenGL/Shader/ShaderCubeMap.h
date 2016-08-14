#pragma once
#include "Shader.h"
#include "../../Color.h"

class ShaderCubeMap : public Shader, public Shader::Loadable
{
public:
	virtual ~ShaderCubeMap() {}
	void load() override;
};
