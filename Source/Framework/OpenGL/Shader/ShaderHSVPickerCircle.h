#pragma once
#include "Shader.h"

class ShaderHSVPickerCircle : public Shader, public Shader::ILoadable
{
public:
	virtual void load() override;
};