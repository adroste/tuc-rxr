#pragma once
#include "../Utility/Point.h"
#include "../System/System.h"

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
	
	static size_t getDrawThreadID();
};

#define DRAW_THREAD assert(System::getThreadID() == Framework::getDrawThreadID());
#define MAIN_THREAD assert(System::getThreadID() != Framework::getDrawThreadID());