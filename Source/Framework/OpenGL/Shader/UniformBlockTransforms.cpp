#include "UniformBlockTransforms.h"

UniformBlockTransforms::UniformBlockTransforms(std::initializer_list<Shader*> refShader, const std::string & blockName)
	:
	UniformBlock(refShader,blockName)
{
	// identity always on bottom
	m_modelMats.push(glm::mat4(1.0));
}

void UniformBlockTransforms::create()
{
	UniformBlock::create();

	if (m_blockSize != sizeof(glm::mat4) * 3)
		throw ExceptionBlockSize("UniformBlockTransforms::create", sizeof(glm::mat4) * 3, m_blockSize);

	// set modelview to uniform
	updateModel();
}

void UniformBlockTransforms::setProjection(const glm::mat4& mat)
{
	updateVar(mat, 0);
}

void UniformBlockTransforms::setCamera(const glm::mat4& mat)
{
	updateVar(mat, sizeof(glm::mat4));
}

void UniformBlockTransforms::pushModel(const glm::mat4& mat)
{
	m_modelMats.push(m_modelMats.top() * mat);
	updateModel();
}

void UniformBlockTransforms::popModel()
{
	assert(m_modelMats.size() > 1);
	m_modelMats.pop();
	updateModel();
}

void UniformBlockTransforms::updateModel()
{
	assert(m_modelMats.size());
	updateVar(m_modelMats.top(), sizeof(glm::mat4) * 2);
}