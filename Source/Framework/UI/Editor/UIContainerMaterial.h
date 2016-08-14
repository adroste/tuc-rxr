#pragma once
#include "../UIItemLister.h"
#include "../UILabel.h"
#include "../UIButtonColor.h"

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
		m_btnSpecular(Color::White())
	{
		// metrics
		float fh = getDFont().getMaxHeight();
		const PointF cdim = { 3.0f * fh,fh };
		m_btnDiffuse.setDim(cdim);
		m_btnSpecular.setDim(cdim);

		// registering
		addToList(&m_lblTitle, nullptr);

		addToList(&m_lblDiffuse, &m_btnDiffuse);
		addToList(&m_lblSpecular, &m_btnSpecular);
		addToList(&m_lblGloss, nullptr);
		addToList(&m_lblShader, nullptr);

		setCellPadding(8.0f);
		setWallPadding(5.0f);
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
	
};
