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

	static const char* names[4] = {
		"LightsNLights",
		"LightsAmbient",
		"LightsEye",
		"LightsLight"
	};

	// determine where in the buffer we should write the values
	glGetUniformIndices(getShaderProgramm(), m_nUniforms, names, m_indices);

	glGetActiveUniformsiv(getShaderProgramm(), m_nUniforms, m_indices, GL_UNIFORM_OFFSET, m_offset);

	glCheck("UniformBlockLight::create");
}

void UniformBlockLight::updateLights(const glm::vec3& ambient, const glm::vec3 eye, const std::vector<LightSource>& src)
{
	// update nLights
	updateVar(int(src.size()), m_offset[0]);
	updateVar(ambient, m_offset[1]);
	updateVar(eye, m_offset[2]);

	assert(src.size() <= MAX_LIGHTS);
	if (src.size())
		updateArray(&src[0], sizeof(LightSource) * std::min(src.size(),MAX_LIGHTS), m_offset[3]);

	flush();
}
