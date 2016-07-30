#pragma once
#include "UniformBlock.h"
#include "../../../glm/detail/type_vec3.hpp"

class UniformBlockMapInfo : public UniformBlock
{
public:
	UniformBlockMapInfo(Shader& refShader, const std::string& blockName);
	virtual ~UniformBlockMapInfo() override;

	void create() override;
	
	void setInfo(const glm::vec3& dim);
};
