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
	UniformBlockLight(std::initializer_list<Shader*> refShader, const std::string& blockName);
	virtual ~UniformBlockLight();
	void create() override;

	void updateLights(const glm::vec3& ambient, const glm::vec3 eye, const std::vector< LightSource >& src);
};
