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

	virtual void loadUniforms() override
	{
		auto tex0 = locateUniform("tex0");
		glUniform1i(tex0, 0);
		auto tex1 = locateUniform("tex1");
		glUniform1i(tex1, 1);
	}
};