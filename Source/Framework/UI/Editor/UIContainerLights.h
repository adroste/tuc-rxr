#pragma once
#include "../UIItemLister.h"
#include "../UILabel.h"
#include "../UIButtonColor.h"
#include "../UIButtonText.h"
#include "../../../Game/Client/GameEditor.h"
#include "../UIDialogColorPicker.h"


class UIContainerLights : public UIItemLister
{
public:
	UIContainerLights(GameEditor& editor)
		:
		UIItemLister(),
		m_editor(editor),
		m_lblTitle(getDFont(),"Lights"),
		m_lblAmbient(getDFont(),"ambient"),
		m_btnAmbient(editor.getAmbientColor()),
		m_btnAddLight(UIButton::Style::Royal,getDFont(),"add light")
	{
		// metrics
		float fh = getDFont().getMaxHeight();
		const PointF cdim = { 5.0f * fh,fh };
		m_btnAmbient.setDim(cdim);

		addToList(&m_lblTitle, nullptr);
		addToList(&m_lblAmbient, &m_btnAmbient);
		addToList(&m_btnAddLight, nullptr);

		setCellPadding(8.0f);
		setWallPadding(5.0f);

		m_colorPicker.setZIndex(1);
		m_colorPicker.registerMe(this);

		m_btnAmbient.setOnClickCallback([this](IClickable*)
		{
			m_colorPicker.show(m_btnAmbient.getValue());
			m_colorPicker.setOnResultCallback([this](UIDialog*)
			{
				m_btnAmbient.setValue(m_colorPicker.getColor());
			});
		});
	}


	virtual void draw(Drawing& draw) override
	{
		UIItemLister::draw(draw);
		pushDrawTransform(draw);
		m_colorPicker.draw(draw);
		popDrawTransform(draw);
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
};
