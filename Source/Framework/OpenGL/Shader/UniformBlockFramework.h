#pragma once
#include "UniformBlock.h"
#include "../../../Utility/Point.h"
#include "../../../Utility/Rect.h"
#include <stack>

class UniformBlockFramework : public UniformBlock
{
public:
	UniformBlockFramework(const std::initializer_list<Shader*>& refShader, const std::string& blockName);
	virtual ~UniformBlockFramework() override;
	void create() override;
	void setMouse(const PointF& mouse);
	virtual void flush() override;
	
	void pushIgnore();
	void pushRect(const RectF& rect);
	void pop();

private:
	void updateModel();
	static RectF getInfiniteRect();
private:
	std::stack<RectF> m_transforms;
	PointF m_mouse;
	bool m_changed = false;
};
