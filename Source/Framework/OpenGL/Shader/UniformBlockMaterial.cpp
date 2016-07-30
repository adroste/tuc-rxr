#include "UniformBlockMaterial.h"
#include "../../../glm/detail/type_vec4.hpp"


UniformBlockMaterial::UniformBlockMaterial(Shader& refShader, const std::string& blockName)
	:
	UniformBlock(refShader, blockName)
{
}

UniformBlockMaterial::~UniformBlockMaterial()
{
}

void UniformBlockMaterial::create()
{
	UniformBlock::create();

	if (m_blockSize != 32)
		throw ExceptionBlockSize("UniformBlockMaterial::create", 32, m_blockSize);
}

void UniformBlockMaterial::updateMaterial(const glm::vec3& diffuse, const glm::vec3& specular, float gloss)
{
	// diffuse
	updateVar(diffuse, 0);
	// specular + gloss
	updateVar(glm::vec4(specular.r,specular.g,specular.b, gloss), 16);

	flush();
}
