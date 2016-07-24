#include "Framework.h"

static PointI m_clientDim;
static PointF m_camDim; // size + borders
static float m_scale;

PointF Framework::convertClientPoint(const PointI& p)
{
	PointF res(float(p.x) / float(m_clientDim.x) * float(m_camDim.x),
		float(p.y) / float(m_clientDim.y) * float(m_camDim.y));

	res.x += (float(STD_DRAW_X) - float(m_camDim.x)) / 2.0f;
	res.y += (float(STD_DRAW_Y) - float(m_camDim.y)) / 2.0f;

	return res;
}

void Framework::setWindowSize(const PointI& clientDim, const PointI& camDim, float scale)
{
	m_clientDim = clientDim;
	m_camDim = camDim;
	m_scale = scale;
}

float Framework::getAspect()
{
	return float(m_clientDim.x) / float(m_clientDim.y);
}
