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
		UIDialog::setDim({ 400,400 });
		setFixedDim(true);

		m_light.setDim({ 380,300 });
		m_light.setOrigin({ 10,10 });
		m_light.orderItems();

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