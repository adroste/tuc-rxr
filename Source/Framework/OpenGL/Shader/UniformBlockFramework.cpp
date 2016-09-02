#include "UniformBlockFramework.h"

UniformBlockFramework::UniformBlockFramework(const std::initializer_list<Shader*>& refShader, const std::string & blockName)
	:
	UniformBlock(refShader,blockName)
{
}

UniformBlockFramework::~UniformBlockFramework()
{
}

void UniformBlockFramework::create()
{
	UniformBlock::create();
	m_changed = true;

	if (m_blockSize != 32)
		throw ExceptionBlockSize("UniformBlockFramework::create", 32, m_blockSize);
}

void UniformBlockFramework::setMouse(const PointF& mouse)
{
	updateVar(mouse, sizeof(RectF));
	m_changed = true;
}

void UniformBlockFramework::flush()
{
	if(m_changed)
	{
		updateModel();
		UniformBlock::flush();
		m_changed = false;
	}
}

void UniformBlockFramework::pushIgnore()
{
	m_transforms.push(getInfiniteRect());
	m_changed = true;
}

void UniformBlockFramework::pushRect(const RectF& rect)
{
	if(m_transforms.size())
	{
		// clip rect with predecessor
		auto clipped = rect.clipTo(m_transforms.top());
		m_transforms.push(clipped);
	}
	else
	{
		m_transforms.push(rect);
	}
	m_changed = true;
}

void UniformBlockFramework::pop()
{
	assert(m_transforms.size());
	m_transforms.pop();
	m_changed = true;
}

void UniformBlockFramework::updateModel()
{
	if(m_transforms.size())
	{
		updateVar(m_transforms.top(), 0);
	}
	else
	{
		// infinite clipping plane
		updateVar(getInfiniteRect(), 0);
	}
}

RectF UniformBlockFramework::getInfiniteRect()
{
	//return RectF(FLT_MIN, FLT_MIN, FLT_MAX, FLT_MAX);
	static const float val = 10000.0f;
	return RectF(-val, -val, val, val);
}
