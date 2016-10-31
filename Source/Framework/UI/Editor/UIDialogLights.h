#pragma once
#include "../UIDialog.h"
#include "UIContainerLights.h"
#include "UILightLister.h"

class UIDialogLights : public UIDialog
{
public:
	UIDialogLights(GameEditor& editor, bool show, WindowManager& wm, size_t anchor = 0, PointF offset = PointF(0.0f))
		:
		UIDialog(Buttons::OKCancel, show),
		m_lights(editor, true, wm),
		m_list(true)
	{
		m_list.setDim({ m_lights.getDim().x,300.0f });
		for (const auto& l : editor.getLights())
		{
			m_list.add(l);
		}
		m_list.adjustToContent();

		auto o = UIDialog::setClientArea(m_lights.getDim() + PointF(0.0f,m_list.getDim().y));
		
		m_lights.setOrigin(o);
		m_lights.orderItems();
		m_list.setOrigin(m_lights.getRect().getBottomLeft());

		m_lights.setZIndex(2);
		m_lights.registerMe(this);
		m_list.setZIndex(1);
		m_list.registerMe(this);

		m_lights.setOnLightAddCallback([this](LightSource s)
		{
			m_list.add(s);
			m_list.adjustToContent();
			UIDialog::setClientArea(m_lights.getDim() + PointF(0.0f, m_list.getDim().y));
		});

		setOnResultProtectedCallback([this, &editor](UIDialog* dlg)
		{
			if (dlg->getResult() == Result::OK)
			{
				// update lights in editor
				editor.updateLights(m_lights.getAmbient(), m_list.getLights());
			}
		});
	}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		UIDialog::draw(draw);

		pushDrawTransforms(draw);
		m_list.draw(draw);
		m_lights.draw(draw);
		popDrawTransforms(draw);
	}
private:
	UIContainerLights m_lights;
	UILightLister m_list;
};
