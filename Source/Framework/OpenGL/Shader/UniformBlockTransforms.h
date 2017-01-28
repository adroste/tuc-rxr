#pragma once
#include "UniformBlock.h"
#include <glm/glm.hpp>
#include <stack>
#include "../../../Utility/Point.h"

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

	PointF modelToWorld(const PointF& m);
private:
	void updateModel();

private:
	std::stack<glm::mat4> m_modelMats;
	bool m_changed = false;
};
