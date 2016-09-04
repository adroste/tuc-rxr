#pragma once
#include "../UIItemLister.h"
#include "../UILabel.h"
#include "../UIButtonColor.h"
#include "../UIInputBox.h"
#include "../UINumUpDownFloat.h"
#include "../UINumUpDownInt.h"
#include "../UINumUpDownCubeShader.h"
#include "../UINumUpDownBlockType.h"

class UIContainerMaterial : public UIItemLister
{     
	CALLBACK(MaterialChange, CubeDesc);
public:
	UIContainerMaterial()
		:
		UIItemLister(),
		m_lblTitle(Drawing::getFont(Font::Style::Headline, Font::Size::M), "Material"),
		m_lblDiffuse(getDFont(), "diffuse"),
		m_lblSpecular(getDFont(), "specular"),
		m_lblGloss(getDFont(), "gloss"),
		m_lblShader(getDFont(), "shader"),
		m_lblBlockType(getDFont(), "type"),
		m_lblGravity(getDFont(), "has gravity"),
		m_lblHP(getDFont(),"HP"),
		m_btnDiffuse(Color::Green()),
		m_btnSpecular(Color::White()),
		m_numSpec(getDFont(), 100.0f, 1.0f, 100000.0f, 1.0f),
		m_numShader(getDFont(), CubeShader::Default, CubeShader::Default, CubeShader(int(CubeShader::Size) - 1),CubeShader(1)),
		m_numHealth(getDFont(),0,0,INT_MAX,10),
		m_numBlockType(getDFont(),CubeDesc::Solid),
		m_boxGravity(false)
	{
		// metrics
		float fh = getDFont().getMaxHeight();
		const PointF cdim = { 5.0f * fh,fh };
		m_btnDiffuse.setDim(cdim);
		m_btnSpecular.setDim(cdim);

		m_boxGravity.setDim({ fh,fh });

		m_numShader.setDim(cdim + PointF(10, 10));
		m_numSpec.setDim(m_numShader.getDim());
		m_numHealth.setDim(m_numShader.getDim());
		m_numBlockType.setDim(m_numShader.getDim());

		// registering
		addToList(&m_lblTitle, nullptr);

		addToList(&m_lblDiffuse, &m_btnDiffuse);
		addToList(&m_lblSpecular, &m_btnSpecular);
		addToList(&m_lblGloss, &m_numSpec);
		addToList(&m_lblShader, &m_numShader);
		addToList(&m_lblBlockType, &m_numBlockType);
		addToList(&m_lblGravity, &m_boxGravity);
		addToList(&m_lblHP, &m_numHealth);

		setCellPadding(8.0f);
		setWallPadding(5.0f);
		adjustToItems();
		setFixedDim(true);

		m_colorPicker.setZIndex(1);
		m_colorPicker.registerMe(this);

		m_btnDiffuse.setOnClickCallback([this](IClickable*)
		{
			m_colorPicker.show(m_btnDiffuse.getValue());
			m_colorPicker.setOnResultCallback([this](UIDialog*)
			{
				m_btnDiffuse.setValue(m_colorPicker.getValue());
				this->updateColor();
			});
		});
		m_btnSpecular.setOnClickCallback([this](IClickable*)
		{
			m_colorPicker.show(m_btnSpecular.getValue());
			m_colorPicker.setOnResultCallback([this](UIDialog*)
			{
				m_btnSpecular.setValue(m_colorPicker.getValue());
				this->updateColor();
			});
		});
		m_numSpec.setOnValueCallback([this](IValueHolder<float>*)
		{
			updateColor();
		});
		m_numShader.setOnValueCallback([this](IValueHolder<CubeShader>*)
		{
			updateColor();
		});
		m_numHealth.setOnValueCallback([this](IValueHolder<int>*)
		{
			updateColor();
		});
		m_boxGravity.setOnValueCallback([this](IValueHolder<bool>*)
		{
			updateColor();
		});
		m_numBlockType.setOnValueCallback([this](IValueHolder<CubeDesc::BlockType>*)
		{
			updateColor();
		});
	}

	virtual void draw(Drawing& draw) override
	{
		UIItemLister::draw(draw);
		pushDrawTransforms(draw);
		m_colorPicker.draw(draw);
		popDrawTransforms(draw);
	}
	CubeDesc getCubeDesc() const
	{
		CubeDesc d;
		d.diffuse = toGamma(m_btnDiffuse.getValue()).toDWORD();
		d.spec = toGamma(m_btnSpecular.getValue()).toDWORD();
		d.gloss = m_numSpec.getValue();
		d.shader = m_numShader.getValue();

		d.blockFlags = 0;
		if (m_boxGravity.getValue()) d.blockFlags |= CubeDesc::Gravity;

		d.blockHP = m_numHealth.getValue();
		d.blockType = m_numBlockType.getValue();
		return d;
	}
	static Color toGamma(const Color& c)
	{
		return Color(std::pow(c.r, 2.2f), std::pow(c.g, 2.2f), std::pow(c.b, 2.2f), c.a);
	}
	static Color fromGamma(const Color& c)
	{
		return Color(std::pow(c.r, 1.0f/2.2f), std::pow(c.g, 1.0f/2.2f), std::pow(c.b, 1.0f/2.2f), c.a);
	}
private:
	// font for description
	static Font& getDFont()
	{
		return Drawing::getFont(Font::Style::Headline, Font::Size::M);
	}
	void updateColor() const
	{
		m_onMaterialChange(getCubeDesc());
	}
private:
	UILabel m_lblTitle;
	UILabel m_lblDiffuse;
	UILabel m_lblSpecular;
	UILabel m_lblGloss;
	UILabel m_lblShader;
	UILabel m_lblBlockType;
	UILabel m_lblGravity;
	UILabel m_lblHP;

	UIButtonColor m_btnDiffuse;
	UIButtonColor m_btnSpecular;

	UINumUpDownFloat m_numSpec;
	UINumUpDownCubeShader m_numShader;
	UINumUpDownInt m_numHealth;
	UINumUpDownBlockType m_numBlockType;

	UICheckBox m_boxGravity;

	UIDialogColorPicker m_colorPicker;
};
