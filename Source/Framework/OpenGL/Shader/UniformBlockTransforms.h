#pragma once
#include "UniformBlock.h"
#include "../../../glm/gtx/transform.hpp"
#include <stack>

class UniformBlockTransforms : public UniformBlock
{
public:
	UniformBlockTransforms(std::initializer_list<Shader*> refShader, const std::string& blockName);
	virtual void create() override;
	virtual void flush() override;
	void setProjection(const glm::mat4& mat);
	void setCamera(const glm::mat4& mat);
	void pushModel(const glm::mat4& mat);
	void popModel();
private:
	void updateModel();

private:
	std::stack<glm::mat4> m_modelMats;
	bool m_changed = false;
};
