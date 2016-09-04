#pragma once
#include "../UIDialog.h"
#include "UIContainerLights.h"

class UIDialogLights : public UIDialog
{
public:
	UIDialogLights(GameEditor& editor)
		:
		UIDialog(Buttons::OKCancel),
		m_lights(editor)
	{
		auto o = UIDialog::setClientArea(m_lights.getDim());
		setFixedDim(true);

		m_lights.setOrigin(o);
		m_lights.orderItems();

		m_lights.registerMe(this);
	}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		UIDialog::draw(draw);

		pushDrawTransforms(draw);
		m_lights.draw(draw);
		popDrawTransforms(draw);
	}
private:
	UIContainerLights m_lights;
};