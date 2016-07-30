#include "UniformBlockTransforms.h"

UniformBlockTransforms::UniformBlockTransforms(Shader & refShader, const std::string & blockName)
	:
	UniformBlock(refShader,blockName)
{
}

void UniformBlockTransforms::create()
{
	UniformBlock::create();

	if (m_blockSize != sizeof(glm::mat4) * 3)
		throw ExceptionBlockSize("UniformBlockTransforms::create", sizeof(glm::mat4) * 3, m_blockSize);
}

void UniformBlockTransforms::setProjection(const glm::mat4& mat)
{
	updateVar(mat, 0);
}

void UniformBlockTransforms::setCamera(const glm::mat4& mat)
{
	updateVar(mat, sizeof(glm::mat4));
}

void UniformBlockTransforms::setModel(const glm::mat4& mat)
{
	updateVar(mat, sizeof(glm::mat4) * 2);
}
