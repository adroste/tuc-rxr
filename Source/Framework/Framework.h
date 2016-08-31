#pragma once
#include "../Utility/Point.h"

class Framework
{
public:
	static const int STD_DRAW_X = 1280;
	static const int STD_DRAW_Y = 720;

public:
	static PointF convertClientPoint(const PointI& p);
	static PointI convertCamPoint(PointF p);
	static void setWindowSize(const PointI& clientDim, const PointI& camDim, float scale);
	static float getAspect();
	static PointF getCamDim();
	static PointF getCamOrigin();
	static PointF getScreenCenter();
	static float getScalar();
};