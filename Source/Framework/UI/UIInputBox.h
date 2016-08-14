#pragma once
#include "UIInputField.h"
#include "UIContainer.h"

class UIInputBox : public UIContainer
{
public:
	UIInputBox(Font& font, size_t maxlen)
		:
		m_field(font, maxlen)
	{
		m_field.registerMe(this);
	}

	virtual void draw(Drawing& draw) override
	{
		// TODO replace with fancy box
		draw.rect(getRect(), Color::White());

		pushDrawTransform(draw);
		m_field.draw(draw);
		popDrawTransform(draw);
	}

	virtual void setDim(const PointF& d) override
	{
		UIContainer::setDim(d);
		updateInputRect();
	}
	virtual void setOrigin(const PointF& p) override
	{
		UIContainer::setOrigin(p);
		updateInputRect();
	}
private:
	void updateInputRect()
	{
		m_field.setOrigin(PointF(border, border));
		m_field.setDim(m_dim - PointF(border, border) * 2.0f);
	}
private:
	UIInputField m_field;
	const float border = 5.0f;
};
