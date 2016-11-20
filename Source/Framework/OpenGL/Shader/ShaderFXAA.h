#pragma once
#include "Shader.h"

class ShaderFXAA : public Shader, public Shader::ILoadable
{
public:
	ShaderFXAA() = default;
	virtual ~ShaderFXAA() override = default;
	
	void load() override
	{
		Shader::load("data/Shader/fxaa");
	}
};