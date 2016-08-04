#pragma once
#include "UIDialog.h"

class UIDialogColorPicker : public UIDialog
{
public:
	UIDialogColorPicker(float width, const PointF& centerPos)
	{
		IMetrics::setDim({ width, width * 2 });
		IMetrics::setCenter(centerPos);
	}

	virtual ~UIDialogColorPicker() override
	{}

	virtual void draw(Drawing& draw) override
	{
		UIDialog::draw(draw);

		draw.hsvPicker(getMidpoint(), getDim().x / 2.0f, Color::Red());
	}

private:
	float m_angle;

};
