#pragma once
#include "Shader.h"
#include "../../Color.h"

class ShaderCube : public Shader
{
public:
	virtual ~ShaderCube() {}
	void load() override;
};
