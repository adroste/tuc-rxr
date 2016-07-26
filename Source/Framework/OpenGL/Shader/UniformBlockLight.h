#pragma once
#include "UniformBlock.h"
#include "../../../glm/detail/type_vec3.hpp"
#include <vector>

class UniformBlockLight : public UniformBlock
{
public:
	struct LightSource
	{
		enum Type
		{
			Directional,
			PointLight
		} type;
		glm::vec3 color;
		glm::vec3 origin;
		float attenuation;
	};
public:
	UniformBlockLight(Shader& refShader, const std::string& blockName);

	virtual ~UniformBlockLight();
	void create() override;

	void updateLights(const glm::vec3& ambient, const glm::vec3 eye, const std::vector< LightSource >& src);
	                                                                             
private:
    static const size_t m_nUniforms = 6;
	GLuint m_indices[m_nUniforms];
	GLint m_offset[m_nUniforms];
};
