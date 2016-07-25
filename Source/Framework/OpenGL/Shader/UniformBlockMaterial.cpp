#include "UniformBlockMaterial.h"


UniformBlockMaterial::UniformBlockMaterial(Shader& refShader, const std::string& blockName)
	:
	UniformBlock(refShader, blockName)
{
}

UniformBlockMaterial::~UniformBlockMaterial()
{
}

void UniformBlockMaterial::create()
{
	UniformBlock::create();

	static const char* names[3] = {
		"MaterialDiffuse",
		"MaterialSpecular",
		"MaterialGlossy"
	};

	// determine where in the buffer we should write the values
	glGetUniformIndices(getShaderProgramm(), m_nUniforms, names, m_indices);

	glGetActiveUniformsiv(getShaderProgramm(), m_nUniforms, m_indices, GL_UNIFORM_OFFSET, m_offset);

	glCheck("UniformBlockMaterial::create");
}

void UniformBlockMaterial::updateMaterial(const glm::vec3& diffuse, const glm::vec3& specular, float gloss)
{
	// diffuse
	memcpy(m_pBuffer.get() + m_offset[0], &diffuse, sizeof(glm::vec3));
	// specular
	memcpy(m_pBuffer.get() + m_offset[1], &specular, sizeof(glm::vec3));
	// gloss
	memcpy(m_pBuffer.get() + m_offset[2], &gloss, sizeof(float));

	flush();
}
