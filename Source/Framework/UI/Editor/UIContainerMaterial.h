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
	CALLBACK(ToBucket, CubeDesc);
public:
	UIContainerMaterial(bool show, WindowManager& wm)
		:
		UIItemLister(show),
		m_lblTitle(Drawing::getFont(Font::Style::Headline, Font::Size::M), "Material"),
		m_lblDiffuse(getDFont(), "diffuse"),
		m_lblSpecular(getDFont(), "specular"),
		m_lblGloss(getDFont(), "gloss"),
		m_lblShader(getDFont(), "shader"),
		m_lblBlockType(getDFont(), "type"),
		m_lblGravity(getDFont(), "has gravity"),
		m_lblHP(getDFont(), "HP"),
		m_btnDiffuse(Color::Green()),
		m_btnSpecular(Color::White()),
		m_numSpec(getDFont(), 100.0f, 1.0f, 100000.0f, 1.0f),
		m_numShader(getDFont(), CubeShader::Default, CubeShader::Default, CubeShader(int(CubeShader::Size) - 1), CubeShader(1)),
		m_numHealth(getDFont(), 0, 0, INT_MAX, 10),
		m_numBlockType(getDFont(), BlockType::Solid),
		m_boxGravity(false),
		m_colorPicker(false, wm),
		m_btnAddBucket(UIButton::Style::Royal, getDFont(), "to bucket")
	{
		// metrics
		float fh = getDFont().getMaxHeight();
		const PointF cdim = { 5.0f * fh,fh };
		m_btnDiffuse->setDim(cdim);
		m_btnSpecular->setDim(cdim);
		m_btnAddBucket->adjustToFontHeadline();

		m_boxGravity->setDim({ fh,fh });

		m_numShader->setDim(cdim + PointF(10, 10));
		m_numSpec->setDim(m_numShader->getDim());
		m_numHealth->setDim(m_numShader->getDim());
		m_numBlockType->setDim(m_numShader->getDim());

		// registering
		addToList(m_lblTitle.getRef(), nullptr);

		addToList(m_lblDiffuse.getRef(), m_btnDiffuse.getRef());
		addToList(m_lblSpecular.getRef(), m_btnSpecular.getRef());
		addToList(m_lblGloss.getRef(), m_numSpec.getRef());
		addToList(m_lblShader.getRef(), m_numShader.getRef());
		addToList(m_lblBlockType.getRef(), m_numBlockType.getRef());
		addToList(m_lblGravity.getRef(), m_boxGravity.getRef());
		addToList(m_lblHP.getRef(), m_numHealth.getRef());
		addToList(m_btnAddBucket.getRef(), nullptr);

		setCellPadding(8.0f);
		setWallPadding(5.0f);
		adjustToItems();

		m_colorPicker.setZIndex(1);
		//m_colorPicker.registerMe(this);

		m_btnDiffuse->setOnClickCallback([this](IClickable*)
		{
			m_colorPicker.show(m_btnDiffuse->getValue());
			m_colorPicker.setOnResultCallback([this](UIDialog*)
			{
				m_btnDiffuse->setValue(m_colorPicker.getValue());
				this->updateColor();
			});
		});
		m_btnSpecular->setOnClickCallback([this](IClickable*)
		{
			m_colorPicker.show(m_btnSpecular->getValue());
			m_colorPicker.setOnResultCallback([this](UIDialog*)
			{
				m_btnSpecular->setValue(m_colorPicker.getValue());
				this->updateColor();
			});
		});
		m_numSpec->setOnValueCallback([this](IValueHolder<float>*)
		{
			updateColor();
		});
		m_numShader->setOnValueCallback([this](IValueHolder<CubeShader>*)
		{
			updateColor();
		});
		m_numHealth->setOnValueCallback([this](IValueHolder<int>*)
		{
			updateColor();
		});
		m_boxGravity->setOnValueCallback([this](IValueHolder<bool>*)
		{
			updateColor();
		});
		m_numBlockType->setOnValueCallback([this](IValueHolder<BlockType>*)
		{
			updateColor();
		});
		m_btnAddBucket->setOnClickCallback([this](IClickable*)
		{
			m_onToBucket(getCubeDesc());
		});
	}

	/*virtual void draw(Drawing& draw) override
	{
		UIItemLister::draw(draw);
		pushDrawTransforms(draw);
		m_colorPicker.draw(draw);
		popDrawTransforms(draw);
	}*/
	CubeDesc getCubeDesc() const
	{
		CubeDesc d;
		d.diffuse = m_btnDiffuse->getValue().toGamma().toDWORD();
		d.spec = m_btnSpecular->getValue().toGamma().toDWORD();
		d.gloss = m_numSpec->getValue();
		d.shader = m_numShader->getValue();

		d.blockFlags = 0;
		if (m_boxGravity->getValue()) d.blockFlags |= CubeDesc::Gravity;

		d.blockHP = m_numHealth->getValue();
		d.blockType = uint8_t(m_numBlockType->getValue());
		return d;
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
	refable<UILabel> m_lblTitle;
	refable<UILabel> m_lblDiffuse;
	refable<UILabel> m_lblSpecular;
	refable<UILabel> m_lblGloss;
	refable<UILabel> m_lblShader;
	refable<UILabel> m_lblBlockType;
	refable<UILabel> m_lblGravity;
	refable<UILabel> m_lblHP;

	refable<UIButtonColor> m_btnDiffuse;
	refable<UIButtonColor> m_btnSpecular;

	refable<UINumUpDownFloat> m_numSpec;
	refable<UINumUpDownCubeShader> m_numShader;
	refable<UINumUpDownInt> m_numHealth;
	refable<UINumUpDownBlockType> m_numBlockType;

	refable<UICheckBox> m_boxGravity;

	UIDialogColorPicker m_colorPicker;
	refable<UIButtonText> m_btnAddBucket;
};
