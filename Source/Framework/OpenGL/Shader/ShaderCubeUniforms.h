#pragma once
#include "Shader.h"

class ShaderCubeUniforms : public Shader
{
protected:
	virtual void loadUniforms() override
	{
		// set texture bindings
		auto mapTexVol = tryLocateUniform("mapTexVol");
		auto texWater = tryLocateUniform("texWater");
		auto texWaterfall = tryLocateUniform("texWaterfall");
		assert(mapTexVol != -1);
		//assert(texWater != -1);
		//assert(texWaterfall != -1);

		glUniform1i(mapTexVol, 0);
		glUniform1i(texWater, 1);
		glUniform1i(texWaterfall, 2);

		glCheck("ShaderCubeUniforms::loadUniforms");
	}
public:
	~ShaderCubeUniforms() override = default;
};