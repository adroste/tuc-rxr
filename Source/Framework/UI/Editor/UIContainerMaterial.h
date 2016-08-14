#pragma once
#include "../UIItemLister.h"
#include "../UILabel.h"

class UIContainerMaterial : public UIItemLister
{           
public:
	UIContainerMaterial()
		:
		UIItemLister(),
		m_lblTitle(Drawing::getFont(Font::Style::Headline, Font::Size::M), "Material"),
		m_lblDiffuse(getDFont(), "diffuse"),
		m_lblGloss(getDFont(), "gloss"),
		m_lblShader(getDFont(), "shader")
	{
		addToList(&m_lblTitle, nullptr);

		addToList(&m_lblDiffuse, nullptr);
		addToList(&m_lblGloss, nullptr);
		addToList(&m_lblShader, nullptr);
	}

private:
	// font for description
	static Font& getDFont()
	{
		return Drawing::getFont(Font::Style::Headline, Font::Size::S);
	}
private:
	UILabel m_lblTitle;
	UILabel m_lblDiffuse;
	UILabel m_lblGloss;
	UILabel m_lblShader;
};
