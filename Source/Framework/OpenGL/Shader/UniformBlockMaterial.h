#pragma once
#include "UniformBlock.h"
#include <glm/glm.hpp>

class UniformBlockMaterial : public UniformBlock
{
public:
	UniformBlockMaterial(std::initializer_list<Shader*> refShader, const std::string& blockName);
	virtual ~UniformBlockMaterial();
	void create() override;

	void updateMaterial(const glm::vec3& diffuse, const glm::vec3& specular, float gloss);
};
