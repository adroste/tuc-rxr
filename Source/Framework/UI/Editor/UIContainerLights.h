#pragma once
#include "../UIItemLister.h"
#include "../UILabel.h"
#include "../UIButtonColor.h"
#include "../UIButtonText.h"
#include "../../../Game/Client/GameEditor.h"
#include "../UIDialogColorPicker.h"
#include "UIDialogLightsAdd.h"

class UIContainerLights : public UIItemLister
{
	CALLBACK(LightAdd, LightSource);
public:
	UIContainerLights(GameEditor& editor, bool show)
		:
		UIItemLister(show),
		m_editor(editor),
		m_lblTitle(getDFont(),"Lights"),
		m_lblAmbient(getDFont(),"ambient"),
		m_btnAmbient(editor.getAmbientColor()),
		m_btnAddLight(UIButton::Style::Royal,getDFont(),"add light"),
		m_colorPicker(false),
		m_dlgLightAdd(false)
	{
		m_btnAmbient.setValue(m_editor.getAmbientColor().fromGamma());
		// metrics
		float fh = getDFont().getMaxHeight();
		const PointF cdim = { 3.0f * fh,fh };
		m_btnAmbient.setDim(cdim);
		m_btnAddLight.adjustToFontHeadline();

		addToList(&m_lblTitle, nullptr);
		addToList(&m_lblAmbient, &m_btnAmbient);
		addToList(&m_btnAddLight, nullptr);

		setCellPadding(8.0f);
		setWallPadding(5.0f);
		adjustToItems();
		setFixedDim(true);

		m_colorPicker.setZIndex(1);
		m_colorPicker.registerMe(this);
		m_dlgLightAdd.setZIndex(1);
		m_dlgLightAdd.registerMe(this);

		m_btnAmbient.setOnClickCallback([this](IClickable*)
		{
			m_colorPicker.show(m_btnAmbient.getValue());
			m_colorPicker.setOnResultCallback([this](UIDialog*)
			{
				m_btnAmbient.setValue(m_colorPicker.getValue());
			});
		});
		m_btnAddLight.setOnClickCallback([this](IClickable*)
		{
			m_dlgLightAdd.setOrigin(getDim() / 2.0f);
			m_dlgLightAdd.show();

		});
		m_dlgLightAdd.setOnResultCallback([this](UIDialog* dlg)
		{
			if(dlg->getResult() == UIDialog::Result::OK)
				m_onLightAdd(m_dlgLightAdd.getLightSource());
		});
	}
	Color getAmbient() const
	{
		return m_btnAmbient.getValue().toGamma();
	}

	virtual void draw(Drawing& draw) override
	{
		UIItemLister::draw(draw);
		pushDrawTransforms(draw);
		m_colorPicker.draw(draw);
		m_dlgLightAdd.draw(draw);
		popDrawTransforms(draw);
	}

private:
	// font for description
	static Font& getDFont()
	{
		return Drawing::getFont(Font::Style::Headline, Font::Size::M);
	}
private:
	GameEditor& m_editor;

	UILabel m_lblTitle;
	UILabel m_lblAmbient;
	UIButtonColor m_btnAmbient;
	UIButtonText m_btnAddLight;

	UIDialogColorPicker m_colorPicker;
	UIDialogLightsAdd m_dlgLightAdd;
};
