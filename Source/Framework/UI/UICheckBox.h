#pragma once
#include "UIObject.h"
#include "Interfaces/IClickable.h"
#include "Interfaces/ILabelable.h"

class UICheckBox : public UIObject, public IClickable, public ILabelable
{
public:
	UICheckBox(const std::string& text = "")
		:
		m_font(Drawing::getFont(Font::Style::Text, Font::Size::M))
	{
		UICheckBox::setText(text);
		
	}

	void draw(Drawing& draw) override
	{
		RectF rct(getOrigin(), getOrigin() + m_boxDim);
		draw.rect(rct, Color::Gray(0.8f));
		draw.rect(rct.shrink(5.0f), Color::Gray(0.1f));

		m_font.write(draw, getText(), m_txtPos);
	}

	virtual void setText(const std::string& text) override
	{
		ILabelable::setText(text);
		calcDim();
	}

protected:
	virtual void calcDim()
	{
		PointF dim = m_boxDim;
		PointF fdim = m_font.getDim(getText());
		assert(fdim.y <= dim.y);
		
		setDim(dim);
	}

	// override as protected in order to disable changing dim from the outside
	virtual void setDim(const PointF& d) override
	{
		UIObject::setDim(d);
	}

private:
	Font& m_font;

	PointF m_boxDim = { 30.0f, 30.0f };
	PointF m_txtPos;
};
