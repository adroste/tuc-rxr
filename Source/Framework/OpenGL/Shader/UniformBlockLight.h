#pragma once
#include "UniformBlock.h"
#include <glm/glm.hpp>
#include <vector>
#include "../../../Game/Shared/LightSource.h"

class UniformBlockLight : public UniformBlock
{
public:
	UniformBlockLight(std::initializer_list<Shader*> refShader, const std::string& blockName);
	virtual ~UniformBlockLight();
	void create() override;

	void updateLights(const glm::vec3& ambient, const glm::vec3 eye, const std::vector< LightSource >& src);
	void updateEye(const glm::vec3& eye);
	void updateGpu();

private:
	bool m_eyeChanged = false;
};
