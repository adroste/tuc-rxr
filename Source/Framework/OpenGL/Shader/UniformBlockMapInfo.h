#pragma once
#include "UniformBlock.h"
#include <glm/glm.hpp>

class UniformBlockMapInfo : public UniformBlock
{
public:
	UniformBlockMapInfo(std::initializer_list<Shader*> refShader, const std::string& blockName);
	virtual ~UniformBlockMapInfo() override;

	void create() override;
	
	void setInfo(const glm::vec3& dim);
};
