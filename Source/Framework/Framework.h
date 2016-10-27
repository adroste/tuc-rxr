#pragma once
#include "../Utility/Point.h"

class Framework
{
public:
	static void setStdDraw(const PointI& sd);
	static PointI getStdDraw();
	static PointI getDefStdDraw();

	static PointF convertClientPoint(const PointI& p);
	static PointI convertCamPoint(PointF p);
	static void setWindowSize(const PointI& clientDim, const PointI& camDim, float scale);
	static float getAspect();
	static PointF getCamDim();
	static PointF getCamOrigin();
	static PointF getScreenCenter();
	static float getScalar();


};