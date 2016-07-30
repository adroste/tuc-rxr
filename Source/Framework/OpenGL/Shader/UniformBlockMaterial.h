#pragma once
#include "UniformBlock.h"
#include "../../../glm/detail/type_vec3.hpp"

class UniformBlockMaterial : public UniformBlock
{
public:
	UniformBlockMaterial(Shader& refShader, const std::string& blockName);
	virtual ~UniformBlockMaterial();
	void create() override;

	void updateMaterial(const glm::vec3& diffuse, const glm::vec3& specular, float gloss);
};
