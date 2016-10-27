#pragma once
#include "../Utility/Rect.h"
#include "../../Framework.h"

class IMetrics
{
public:
	virtual ~IMetrics() {}

	virtual void setOrigin(const PointF& p)
	{
		m_pos = p;
	}
	virtual void setCenter(const PointF& p)
	{
		setOrigin(p - (m_dim / 2.0f));
	}
	virtual const PointF& getOrigin() const
	{
		return m_pos;
	}
	virtual void setDim(const PointF& d)
	{
		m_dim = d;
	}
	virtual const PointF& getDim() const
	{
		return m_dim;
	}
	virtual void centerX(float y)
	{
		m_pos.y = y;
		m_pos.x = (float(Framework::getStdDraw().x) / 2.0f) - (m_dim.x / 2.0f);
		setOrigin(m_pos);
	}
	virtual void center()
	{
		auto sd = Framework::getStdDraw();
		m_pos.y = (float(sd.y) / 2.0f) - (m_dim.y / 2.0f);
		m_pos.x = (float(sd.x) / 2.0f) - (m_dim.x / 2.0f);
		setOrigin(m_pos);
	}

	RectF getRect() const
	{
		return RectF(m_pos, m_pos + m_dim);
	}
	virtual void setWidth(float w)
	{
		setDim({ w, m_dim.y });
	}
	virtual void setHeight(float h)
	{
		setDim({ m_dim.x, h });
	}

	virtual void setLeft(float l)
	{
		PointF p = getOrigin();
		p.x = Framework::getCamOrigin().x + l;
		setOrigin(p);
	}

	virtual void setRight(float r)
	{
		PointF p = getOrigin();
		p.x = Framework::getCamOrigin().x + Framework::getCamDim().x - r - getDim().x;
		setOrigin(p);
	}

	virtual void setTop(float t)
	{
		PointF p = getOrigin();
		p.y = Framework::getCamOrigin().y + t;
		setOrigin(p);
	}

	virtual void setBottom(float b)
	{
		PointF p = getOrigin();
		p.y = Framework::getCamOrigin().y + Framework::getCamDim().y - b - getDim().y;
		setOrigin(p);
	}

	PointF getMidpoint() const
	{
		return PointF(m_pos + (m_dim / 2.0f));
	}

	/* 
	*	shrinks (p > 0) or grows (p < 0) dims + moves origin
	*	PointF pad: 
	*				pad.x - left / right padding
	*				pad.y - top / bottom padding
	*/
	virtual void addPadding(PointF pad) 
	{
		setDim(getDim() - pad * 2.0f);
		setOrigin(getOrigin() + pad);
	}

private:
	PointF m_pos;
	PointF m_dim;
};
