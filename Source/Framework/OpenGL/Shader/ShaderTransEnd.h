#pragma once
#include "Shader.h"

class ShaderTransEnd : public Shader, public Shader::ILoadable
{
public:
	ShaderTransEnd() = default;
	virtual ~ShaderTransEnd() override = default;

	void load() override
	{
		Shader::load("data/Shader/trans_end");
	}
};