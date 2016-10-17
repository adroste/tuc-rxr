#pragma once
#include "../UIItemLister.h"
#include "../UINumUpDownLightSource.h"
#include "../UIButtonColor.h"
#include "../UINumUpDownFloat.h"
#include "../UILabel.h"

// this container shows properties for directional/point light
class UIContainerLightAdd : public UIItemLister 
{
public:
	UIContainerLightAdd(bool show)
		:
		UIItemLister(show),
		m_type(getDFont(),LightType::Directional),
		m_color(Color::White()),
		m_attenuation(getDFont(),0.2f,0.00000000000001f,100.0f,0.01f),
		m_x(getDFont(),0.0f, -FLT_MAX,FLT_MAX,1.0f),
		m_y(getDFont(),0.0f, -FLT_MAX,FLT_MAX,1.0f),
		m_z(getDFont(),0.0f, -FLT_MAX,FLT_MAX,1.0f),
		
		m_lblType(getDFont(),"Type"),
		m_lblColor(getDFont(), "Color"),
		m_lblAttenuation(getDFont(), "Attenuation"),
		m_lblOrigin(getDFont(), "Direction"),
		m_lblX(getDFont(), "x"),
		m_lblY(getDFont(), "y"),
		m_lblZ(getDFont(), "z"),

		m_dlgColor(false)
	{
		// metrics
		float fh = getDFont().getMaxHeight();
		const PointF cdim = { 5.0f * fh,fh };

		m_color.setDim(cdim);

		m_attenuation.setDim(cdim + PointF(10, 10));
		m_x.setDim(m_attenuation.getDim());
		m_y.setDim(m_attenuation.getDim());
		m_z.setDim(m_attenuation.getDim());
		m_type.setDim(m_attenuation.getDim());

		addToList(&m_lblType, &m_type);
		addToList(&m_lblColor, &m_color);
		addToList(&m_lblOrigin, nullptr);
		addToList(&m_lblX, &m_x);
		addToList(&m_lblY, &m_y);
		addToList(&m_lblZ, &m_z);
		addToList(&m_lblAttenuation, &m_attenuation);

		setCellPadding(8.0f);
		setWallPadding(5.0f);
		adjustToItems();
		setFixedDim(true);

		m_dlgColor.setZIndex(1);
		m_dlgColor.registerMe(this);
		m_color.setOnClickCallback([this](IClickable*)
		{
			m_dlgColor.setCenter(getRect().getMidpoint());
			m_dlgColor.show(m_color.getValue());
			m_dlgColor.setOnResultCallback([this](UIDialog* dlg)
			{
				if (dlg->getResult() == UIDialog::Result::OK)
					m_color.setValue(m_dlgColor.getValue());
			});
		});

		m_type.setOnValueCallback([this](IValueHolder<LightType>* vh)
		{
			switch(vh->getValue())
			{
			case LightType::Directional:
				m_lblOrigin.setText("Direction");
				m_lblAttenuation.disable();
				m_attenuation.disable();
				break;
			case LightType::PointLight: 
				m_lblOrigin.setText("Origin");
				m_lblAttenuation.enable();
				m_attenuation.enable();
				break;
			}
		});
	}
	LightSource getLightSource() const
	{
		LightSource s;
		s.origin = glm::vec3(m_x.getValue(), m_y.getValue(), m_z.getValue());
		s.attenuation = m_attenuation.getValue();
		s.color = m_color.getValue().toGamma().toVec3();
		s.type = m_type.getValue();
		return s;
	}

	virtual void draw(Drawing& draw) override
	{
		UIItemLister::draw(draw);
		pushDrawTransforms(draw, PointF(0.0f, 0.0f));
		m_dlgColor.draw(draw);
		popDrawTransforms(draw);
	}

	void normalizeOrigin()
	{
		auto v = glm::vec3(m_x.getValue(), m_y.getValue(), m_z.getValue());
		v = glm::normalize(v);
		m_x.setValue(v.x);
		m_y.setValue(v.y);
		m_z.setValue(v.z);
	}
private:
	static Font& getDFont()
	{
		return Drawing::getFont(Font::Style::Headline, Font::Size::M);
	}
public:
	UINumUpDownLightSource m_type;
	UIButtonColor m_color;
	UINumUpDownFloat m_attenuation;
	UINumUpDownFloat m_x;
	UINumUpDownFloat m_y;
	UINumUpDownFloat m_z;

	UILabel m_lblType;
	UILabel m_lblColor;
	UILabel m_lblAttenuation;
	UILabel m_lblOrigin;
	UILabel m_lblX;
	UILabel m_lblY;
	UILabel m_lblZ;

	UIDialogColorPicker m_dlgColor;
};