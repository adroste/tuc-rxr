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
	UIContainerLightAdd(bool show, WindowManager& wm)
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

		m_dlgColor(false, wm)
	{
		// metrics
		float fh = getDFont().getMaxHeight();
		const PointF cdim = { 5.0f * fh,fh };

		m_color->setDim(cdim);

		m_attenuation->setDim(cdim + PointF(10, 10));
		m_x->setDim(m_attenuation->getDim());
		m_y->setDim(m_attenuation->getDim());
		m_z->setDim(m_attenuation->getDim());
		m_type->setDim(m_attenuation->getDim());

		addToList(m_lblType.getRef(), m_type.getRef());
		addToList(m_lblColor.getRef(), m_color.getRef());
		addToList(m_lblOrigin.getRef(), nullptr);
		addToList(m_lblX.getRef(), m_x.getRef());
		addToList(m_lblY.getRef(), m_y.getRef());
		addToList(m_lblZ.getRef(), m_z.getRef());
		addToList(m_lblAttenuation.getRef(), m_attenuation.getRef());

		setCellPadding(8.0f);
		setWallPadding(5.0f);
		adjustToItems();
		setFixedDim(true);

		m_dlgColor.setZIndex(1);
		//m_dlgColor.registerMe(this);
		m_color->setOnClickCallback([this](IClickable*)
		{
			m_dlgColor.setCenter(getRect().getMidpoint());
			m_dlgColor.show(m_color->getValue());
			m_dlgColor.setOnResultCallback([this](UIDialog* dlg)
			{
				if (dlg->getResult() == UIDialog::Result::OK)
					m_color->setValue(m_dlgColor.getValue());
			});
		});

		m_type->setOnValueCallback([this](IValueHolder<LightType>* vh)
		{
			switch(vh->getValue())
			{
			case LightType::Directional:
				m_lblOrigin->setText("Direction");
				m_lblAttenuation->disable();
				m_attenuation->disable();
				break;
			case LightType::PointLight: 
				m_lblOrigin->setText("Origin");
				m_lblAttenuation->enable();
				m_attenuation->enable();
				break;
			}
		});
	}
	LightSource getLightSource() const
	{
		LightSource s;
		s.origin = glm::vec3(m_x->getValue(), m_y->getValue(), m_z->getValue());
		s.attenuation = m_attenuation->getValue();
		s.color = m_color->getValue().toGamma().toVec3();
		s.type = m_type->getValue();
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
		auto v = glm::vec3(m_x->getValue(), m_y->getValue(), m_z->getValue());
		v = glm::normalize(v);
		m_x->setValue(v.x);
		m_y->setValue(v.y);
		m_z->setValue(v.z);
	}
private:
	static Font& getDFont()
	{
		return Drawing::getFont(Font::Style::Headline, Font::Size::M);
	}
public:
	refable<UINumUpDownLightSource> m_type;
	refable<UIButtonColor> m_color;
	refable<UINumUpDownFloat> m_attenuation;
	refable<UINumUpDownFloat> m_x;
	refable<UINumUpDownFloat> m_y;
	refable<UINumUpDownFloat> m_z;

	refable<UILabel> m_lblType;
	refable<UILabel> m_lblColor;
	refable<UILabel> m_lblAttenuation;
	refable<UILabel> m_lblOrigin;
	refable<UILabel> m_lblX;
	refable<UILabel> m_lblY;
	refable<UILabel> m_lblZ;

	UIDialogColorPicker m_dlgColor;
};