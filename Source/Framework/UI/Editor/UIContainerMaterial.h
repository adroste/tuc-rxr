#pragma once
#include "../UIItemLister.h"
#include "../UILabel.h"
#include "../UIButtonColor.h"
#include "../UIInputBox.h"
#include "../UINumUpDownFloat.h"
#include "../UINumUpDownInt.h"
#include "../UINumUpDownCubeShader.h"

class UIContainerMaterial : public UIItemLister
{           
public:
	UIContainerMaterial()
		:
		UIItemLister(),
		m_lblTitle(Drawing::getFont(Font::Style::Headline, Font::Size::M), "Material"),
		m_lblDiffuse(getDFont(), "diffuse"),
		m_lblSpecular(getDFont(), "specular"),
		m_lblGloss(getDFont(), "gloss"),
		m_lblShader(getDFont(), "shader"),
		m_btnDiffuse(Color::Green()),
		m_btnSpecular(Color::White()),
		m_numSpec(getDFont(), 100.0f, 1.0f, 100000.0f, 1.0f),
		m_numShader(getDFont(), CubeShader::Default, CubeShader::Default, CubeShader(int(CubeShader::Size) - 1),CubeShader(1))
	{
		// metrics
		float fh = getDFont().getMaxHeight();
		const PointF cdim = { 5.0f * fh,fh };
		m_btnDiffuse.setDim(cdim);
		m_btnSpecular.setDim(cdim);

		m_numShader.setDim(cdim + PointF(10, 10));
		m_numSpec.setDim(m_numShader.getDim());

		// registering
		addToList(&m_lblTitle, nullptr);

		addToList(&m_lblDiffuse, &m_btnDiffuse);
		addToList(&m_lblSpecular, &m_btnSpecular);
		addToList(&m_lblGloss, &m_numSpec);
		addToList(&m_lblShader, &m_numShader);

		setCellPadding(8.0f);
		setWallPadding(5.0f);

		m_colorPicker.setZIndex(1);
		m_colorPicker.registerMe(this);

		m_btnDiffuse.setOnClickCallback([this](IClickable*)
		{
			m_colorPicker.show(m_btnDiffuse.getValue());
			m_colorPicker.setOnResultCallback([this](UIDialog*)
			{
				m_btnDiffuse.setValue(m_colorPicker.getColor());
			});
		});
		m_btnSpecular.setOnClickCallback([this](IClickable*)
		{
			m_colorPicker.show(m_btnSpecular.getValue());
			m_colorPicker.setOnResultCallback([this](UIDialog*)
			{
				m_btnSpecular.setValue(m_colorPicker.getColor());
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
	UILabel m_lblTitle;
	UILabel m_lblDiffuse;
	UILabel m_lblSpecular;
	UILabel m_lblGloss;
	UILabel m_lblShader;

	UIButtonColor m_btnDiffuse;
	UIButtonColor m_btnSpecular;

	UINumUpDownFloat m_numSpec;
	UINumUpDownCubeShader m_numShader;

	UIDialogColorPicker m_colorPicker;
};
