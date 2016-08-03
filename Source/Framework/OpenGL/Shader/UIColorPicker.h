#pragma once
#include "../../UI/UIObject.h"

class UIColorPicker : public UIObject, public Input::IReceiver
{
public:
	UIColorPicker(float width, const PointF& centerPos)
	{
		IMetrics::setDim({ width, width * 2 });
		IMetrics::setCenter(centerPos);
	}

	virtual ~UIColorPicker()
	{}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		draw.hsvPicker(getMidpoint(), getDim().x / 2.0f, Color::Red());
	}

private:
	float m_angle;

};