#pragma once
#include "UniformBlock.h"
#include "../../../glm/gtx/transform.hpp"

class UniformBlockTransforms : public UniformBlock
{
public:
	UniformBlockTransforms(Shader& refShader, const std::string& blockName);
	virtual void create() override;
	void setProjection(const glm::mat4& mat);
	void setCamera(const glm::mat4& mat);
	void setModel(const glm::mat4& mat);
private:
	static const size_t m_nUniforms = 3;
	GLuint m_indices[m_nUniforms];
	GLint m_size[m_nUniforms];
	GLint m_offset[m_nUniforms];
	GLint m_type[m_nUniforms];
};
