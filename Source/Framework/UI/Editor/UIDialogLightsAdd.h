#pragma once
#include "../UIDialog.h"
#include "UIContainerLightAdd.h"

class UIDialogLightsAdd : public UIDialog
{
public:
	UIDialogLightsAdd(bool show)
		:
		UIDialog(Buttons::OKCancel, show),
		m_light(true)
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
	LightSource getLightSource() const
	{
		return m_light.getLightSource();
	}

protected:
	virtual void setResult(Result dlgResult) override
	{
		if (dlgResult == Result::OK)
		{
			// check if light source makes sense
			auto l = getLightSource();
			if(l.type == LightSource::Directional)
			{
				if(glm::length(l.origin) > 0.0000001f)
				{
					m_light.normalizeOrigin();
				}
				else
				{
					System::messageBox("invalid direction", "direction may not be (0|0|0)", System::BoxIcon::Error);
					return;
				}
			}
		}
		
		UIDialog::setResult(dlgResult);
	}
private:
	UIContainerLightAdd m_light;
};