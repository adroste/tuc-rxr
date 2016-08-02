#pragma once
#include "UIButton.h"
#include "Interfaces/ILabelable.h"

class UIButtonText : public UIButton, public ILabelable
{
public:
	UIButtonText(Style style, Font& font, const std::string& text)
		:
		UIButton(style),
		m_font(font)
	{
		UIButtonText::setText(text);
	}

	virtual ~UIButtonText()
	{
	}

	virtual void draw(Drawing& draw) override
	{
		switch (m_style)
		{
		case Style::Royal:
			draw.buttonRoyal(getRect(), m_isDown);
			m_font.write(getText(), m_textPosRel + getOrigin());
			break;
		default:
			throw Exception("UIButtonText::draw default case, missing style");
		}
	}

	virtual void adjustToFontHeadline()
	{
		PointF offset;
		switch (m_style)
		{
		case Style::Royal:
			offset.x = 1.45f;
			offset.y = 1.7;
			break;
		default:
			throw Exception("UIButtonText::adjustToFontHeadline default case, missing style");
		}

		setDim(PointF::mult(m_font.getDim(getText()), offset));
	}


	virtual void setDim(const PointF& d) override
	{
		IMetrics::setDim(d);
		calcTextPosition();
	}

	virtual void setText(const std::string& text) override
	{
		ILabelable::setText(text);
		calcTextPosition();
	}

protected:
	void calcTextPosition()
	{
		m_textPosRel = (getDim() - m_font.getDim(getText())) / 2;
	}

protected:
	Font& m_font;
	PointF m_textPosRel;
};
