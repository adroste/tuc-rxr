#include "UniformBlockLight.h"
#include <algorithm>

static const size_t MAX_LIGHTS = 20;

UniformBlockLight::UniformBlockLight(Shader& refShader, const std::string& blockName)
	: UniformBlock(refShader, blockName)
{
}

UniformBlockLight::~UniformBlockLight()
{
}

void UniformBlockLight::create()
{
	UniformBlock::create();

	static const char* names[] = {
		"LightsNLights",
		"LightsAmbient",
		"LightsEye",
		"LightsLight[0]",
		"LightsLight[1]"
	};

	// determine where in the buffer we should write the values
	glGetUniformIndices(getShaderProgramm(), 3, names, m_indices);
	m_indices[3] = 3;
	m_indices[4] = 4;
	m_indices[5] = 5;

	glGetActiveUniformsiv(getShaderProgramm(), 6, m_indices, GL_UNIFORM_OFFSET, m_offset);

	glCheck("UniformBlockLight::create");
}

void UniformBlockLight::updateLights(const glm::vec3& ambient, const glm::vec3 eye, const std::vector<LightSource>& src)
{
	// structure in memory:

	/*
	layout( std140 ) uniform Lights
	{
		uint LightsNLights;						0  - 16
		vec3 LightsAmbient;						16 - 32
		vec3 LightsEye;							32 - 48
		LightSource LightsLight[MAX_LIGHTS];	48 - 96 | 96 - 144 ...
	};
	*/

	// update nLights
	updateVar(int(src.size()), 0);
	updateVar(ambient, 16);
	updateVar(eye, 32);

	assert(src.size() <= MAX_LIGHTS);
	if (src.size())
	{
		size_t curOff = 48;
		for(const auto& s : src)
		{
			updateVar(s.type, curOff);
			updateVar(s.attenuation, curOff + 4);

			updateVar(s.color, curOff + 16);
			updateVar(s.origin, curOff + 32);
			curOff += 48;
		}
	}

	flush();
	glCheck("UniformBlockLight::updateLights");
}
