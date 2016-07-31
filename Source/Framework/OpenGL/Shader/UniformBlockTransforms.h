#pragma once
#include "UniformBlock.h"
#include "../../../glm/gtx/transform.hpp"

class UniformBlockTransforms : public UniformBlock
{
public:
	UniformBlockTransforms(std::initializer_list<Shader*> refShader, const std::string& blockName);
	virtual void create() override;
	void setProjection(const glm::mat4& mat);
	void setCamera(const glm::mat4& mat);
	void setModel(const glm::mat4& mat);
};
