#pragma once
#include "UIObject.h"
#include "Interfaces/ILableable.h"

class UILable : public UIObject, public ILableable
{
public:
	UILable(Font& font, const std::string& txt)
		:
		m_font(font)
	{
		UILable::setText(txt);
	}
	virtual ~UILable() override
	{

	}
	virtual void draw(Drawing& draw) override
	{
		m_font.write(getText(), m_pos);
	}

	virtual void setText(const std::string& text) override
	{
		m_dim = m_font.getDim(text);
		ILableable::setText(text);
	}
private:
	Font& m_font;
};
