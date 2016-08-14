#pragma once
#include "Shader.h"

class ShaderHSVPicker : public Shader, public Shader::Loadable
{
public:
	virtual void load() override;
};