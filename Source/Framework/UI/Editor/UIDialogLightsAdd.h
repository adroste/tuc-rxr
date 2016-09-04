#pragma once
#include "../UIDialog.h"
#include "UIContainerLightAdd.h"

class UIDialogLightsAdd : public UIDialog
{
public:
	UIDialogLightsAdd()
		:
		UIDialog(Buttons::OKCancel),
		m_light()
	{
		auto o = UIDialog::setClientArea(m_light.getDim());
		setFixedDim(true);

		m_light.setOrigin(o);
		m_light.orderItems();

		m_light.setZIndex(1);
		m_light.registerMe(this);
	}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		UIDialog::draw(draw);

		pushDrawTransforms(draw);
		m_light.draw(draw);
		popDrawTransforms(draw);
	}
private:
	UIContainerLightAdd m_light;
};