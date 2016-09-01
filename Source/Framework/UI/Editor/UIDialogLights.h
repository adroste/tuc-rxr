#pragma once
#include "../UIDialog.h"
#include "UIContainerLights.h"

class UIDialogLights : public UIDialog
{
public:
	UIDialogLights(GameEditor& editor)
		:
		UIDialog(),
		m_lights(editor)
	{
		UIDialog::setDim({ 300,400 });
		setFixedDim(true);

		m_lights.setDim({ 280,300 });
		m_lights.setOrigin({ 10,10 });
		m_lights.orderItems();

		m_lights.registerMe(this);
	}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		UIDialog::draw(draw);

		pushDrawTransform(draw);
		m_lights.draw(draw);
		popDrawTransform(draw);
	}
private:
	UIContainerLights m_lights;
};