#pragma once
#include "UniformBlock.h"
#include <glm/glm.hpp>
#include "../../Framework.h"

class UniformBlockFramebuffer : public UniformBlock
{
public:

	UniformBlockFramebuffer(const std::initializer_list<Shader*>& refShader, const std::string& blockName)
		: UniformBlock(refShader, blockName)
	{
	}

	virtual ~UniformBlockFramebuffer() override = default;
	void create() override
	{
		UniformBlock::create();
		if (m_blockSize != 16)
			throw ExceptionBlockSize("UniformBlockFramebuffer::create", 16, m_blockSize);
	}
	void setStep(glm::vec2 step)
	{
		DRAW_THREAD;
		updateVar(step, 0);
		flush();
	}
};
