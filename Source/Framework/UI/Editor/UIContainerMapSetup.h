#pragma once
#include "../UIItemLister.h"
#include "../UILabel.h"
#include "../UINumUpDownInt.h"

class UIContainerMapSetup : public UIItemLister 
{
public:
	UIContainerMapSetup(bool show)
		:
		UIItemLister(show),
		m_lblTitle(getFont(),"Map Size"),
		m_lblWi(getFont(),"width"),
		m_lblHi(getFont(),"height"),
		m_lblDe(getFont(),"depth"),

		m_numX(getFont(),10,10,INT_MAX,10),
		m_numY(getFont(),10,10,INT_MAX,10),
		m_numZ(getFont(),8,4,INT_MAX,1)
	{
		float fh = getFont().getMaxHeight();
		const PointF cdim = { 5.0f * fh,fh };

		m_numX->setDim(cdim + PointF(10.0f));
		m_numY->setDim(m_numX->getDim());
		m_numZ->setDim(m_numX->getDim());

		addToList(m_lblTitle.getRef(), nullptr);

		addToList(m_lblWi.getRef(), m_numX.getRef());
		addToList(m_lblHi.getRef(), m_numY.getRef());
		addToList(m_lblDe.getRef(), m_numZ.getRef());

		setCellPadding(8.0f);
		setWallPadding(5.0f);
		adjustToItems();
	}
	Point3S getMapdim() const
	{
		return Point3S(size_t(m_numX->getValue()), size_t(m_numY->getValue()), size_t(m_numZ->getValue()));
	}
private:
	static Font& getFont()
	{
		return Drawing::getFont(Font::Style::Headline, Font::Size::M);
	}
private:
	refable<UILabel> m_lblTitle;
	refable<UILabel> m_lblWi;
	refable<UILabel> m_lblHi;
	refable<UILabel> m_lblDe;
	
	refable<UINumUpDownInt> m_numX;
	refable<UINumUpDownInt> m_numY;
	refable<UINumUpDownInt> m_numZ;
};
