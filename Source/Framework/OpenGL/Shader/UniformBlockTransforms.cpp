#include "UniformBlockTransforms.h"

UniformBlockTransforms::UniformBlockTransforms(Shader & refShader, const std::string & blockName)
	:
	UniformBlock(refShader,blockName)
{
}

void UniformBlockTransforms::create()
{
	UniformBlock::create();

	static const char* names[3] = {
		"matProjection",
		"matCamera",
		"matModel"
	};

	// determine where in the buffer we should write the values
	glGetUniformIndices(getShaderProgramm(), m_nUniforms, names, m_indices);

	glGetActiveUniformsiv(getShaderProgramm(), m_nUniforms, m_indices, GL_UNIFORM_OFFSET, m_offset);
	glGetActiveUniformsiv(getShaderProgramm(), m_nUniforms, m_indices, GL_UNIFORM_SIZE, m_size);
	glGetActiveUniformsiv(getShaderProgramm(), m_nUniforms, m_indices, GL_UNIFORM_TYPE, m_type);

	glCheck("UniformBlockTransforms::create");
}

void UniformBlockTransforms::setProjection(const glm::mat4& mat)
{

	memcpy(m_pBuffer.get() + m_offset[0], &mat[0][0], sizeof(glm::mat4));
}

void UniformBlockTransforms::setCamera(const glm::mat4& mat)
{
	memcpy(m_pBuffer.get() + m_offset[1], &mat[0][0], sizeof(glm::mat4));
}

void UniformBlockTransforms::setModel(const glm::mat4& mat)
{
	memcpy(m_pBuffer.get() + m_offset[2], &mat[0][0], sizeof(glm::mat4));
}
