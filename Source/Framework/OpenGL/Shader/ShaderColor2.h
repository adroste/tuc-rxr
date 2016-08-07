#pragma once
#include "Shader.h"

class ShaderColor2 : public Shader
{
public:
	~ShaderColor2() override
	{}
	virtual void load() override
	{
		Shader::load("data/Shader/color2");
	}
};