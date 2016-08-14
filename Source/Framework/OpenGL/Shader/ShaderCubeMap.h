#pragma once
#include "Shader.h"
#include "../../Color.h"

class ShaderCubeMap : public Shader
{
public:
	virtual ~ShaderCubeMap() {}
	void load() override;
};
