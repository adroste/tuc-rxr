#include "UniformBlockMapInfo.h"

UniformBlockMapInfo::UniformBlockMapInfo(Shader & refShader, const std::string & blockName)
	: 
	UniformBlock(refShader, blockName) {
}

UniformBlockMapInfo::~UniformBlockMapInfo()
{
}

void UniformBlockMapInfo::create()
{
	UniformBlock::create();

	if (m_blockSize != 16)
		throw ExceptionBlockSize("UniformBlockMapInfo::create", 16, m_blockSize);
}

void UniformBlockMapInfo::setInfo(const glm::vec3& dim)
{
	updateVar(dim, 0);
	flush();
}
