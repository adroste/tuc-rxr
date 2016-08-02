#include "UniformBlockFramework.h"

UniformBlockFramework::UniformBlockFramework(const std::initializer_list<Shader*>& refShader, const std::string & blockName)
	:
	UniformBlock(refShader,blockName)
{
}

UniformBlockFramework::~UniformBlockFramework()
{
}

void UniformBlockFramework::create()
{
	UniformBlock::create();

	if (m_blockSize != 16)
		throw ExceptionBlockSize("UniformBlockFramework::create", 16, m_blockSize);
}

void UniformBlockFramework::update(const PointF& mouse)
{
	updateVar(mouse, 0);
	flush();
}
