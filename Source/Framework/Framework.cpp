#include "Framework.h"
#include "../System/Exceptions/ExceptionArgument.h"
#include "OpenGL/Graphics.h"

static const PointI DEF_STD_DRAW(1280, 720);
static PointI m_stdDraw = DEF_STD_DRAW;
static PointI m_clientDim = DEF_STD_DRAW;
static PointF m_camDim; // size + borders
static float m_scale;

void Framework::setStdDraw(const PointI& sd)
{
	if (sd.x == 0 || sd.y == 0)
		throw new ExceptionArgument("Framework::setStdDraw stdDraw values can't be zero", "sd.x == 0 | sd.y == 0");
	m_stdDraw = sd;
	Graphics::getCurrentGfx()->resize(m_clientDim);
}

PointI Framework::getStdDraw()
{
	return m_stdDraw;
}

PointI Framework::getDefStdDraw()
{
	return DEF_STD_DRAW;
}

PointF Framework::convertClientPoint(const PointI& p)
{
	PointF res(float(p.x) / float(m_clientDim.x) * float(m_camDim.x),
		float(p.y) / float(m_clientDim.y) * float(m_camDim.y));

	auto sd = getStdDraw();
	res.x += (float(sd.x) - float(m_camDim.x)) / 2.0f;
	res.y += (float(sd.y) - float(m_camDim.y)) / 2.0f;

	return res;
}

PointI Framework::convertCamPoint(PointF p)
{
	auto sd = getStdDraw();
	p.x -= (float(sd.x) - float(m_camDim.x)) / 2.0f;
	p.y -= (float(sd.y) - float(m_camDim.y)) / 2.0f;

	p.x = p.x * float(m_clientDim.x) / float(m_camDim.x);
	p.y = p.y * float(m_clientDim.y) / float(m_camDim.y);

	return p;
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

PointF Framework::getCamDim()
{
	return m_camDim;
}

PointF Framework::getCamOrigin()
{
	return (PointF(getStdDraw()) - m_camDim) / 2.0f;
}

PointF Framework::getScreenCenter()
{
	return PointF(Framework::getStdDraw()) / 2.0f;
}

float Framework::getScalar()
{
	return m_scale;
}
