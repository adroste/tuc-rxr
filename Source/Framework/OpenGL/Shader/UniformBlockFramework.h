#pragma once
#include "UniformBlock.h"
#include "../../../Utility/Point.h"

class UniformBlockFramework : public UniformBlock
{
public:
	UniformBlockFramework(const std::initializer_list<Shader*>& refShader, const std::string& blockName);
	virtual ~UniformBlockFramework() override;
	void create() override;
	void update(const PointF& mouse);
};
