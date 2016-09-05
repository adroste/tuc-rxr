#pragma once
#include "../UIItemLister.h"
#include "../UILabel.h"
#include "../UINumUpDownInt.h"

class UIContainerMapSetup : public UIItemLister 
{
public:
	UIContainerMapSetup()
		:
		UIItemLister(),
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

		m_numX.setDim(cdim + PointF(10.0f));
		m_numY.setDim(m_numX.getDim());
		m_numZ.setDim(m_numX.getDim());

		addToList(&m_lblTitle, nullptr);

		addToList(&m_lblWi, &m_numX);
		addToList(&m_lblHi, &m_numY);
		addToList(&m_lblDe, &m_numZ);

		setCellPadding(8.0f);
		setWallPadding(5.0f);
		adjustToItems();
	}
	Point3S getMapdim() const
	{
		return Point3S(size_t(m_numX.getValue()), size_t(m_numY.getValue()), size_t(m_numZ.getValue()));
	}
private:
	static Font& getFont()
	{
		return Drawing::getFont(Font::Style::Headline, Font::Size::M);
	}
private:
	UILabel m_lblTitle;
	UILabel m_lblWi;
	UILabel m_lblHi;
	UILabel m_lblDe;

	UINumUpDownInt m_numX;
	UINumUpDownInt m_numY;
	UINumUpDownInt m_numZ;
};
