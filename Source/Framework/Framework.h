#pragma once
#include "../Utility/Point.h"

class Framework
{
public:
	static const int STD_DRAW_X = 1280;
	static const int STD_DRAW_Y = 720;

public:
	static PointF convertClientPoint(const PointI& p);
	static void setWindowSize(const PointI& clientDim, const PointI& camDim, float scale);
};