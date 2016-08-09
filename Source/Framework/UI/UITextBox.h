#pragma once
#include "UIObject.h"
#include "Interfaces/ILabelable.h"

class UITextBox : public UIObject, public ILabelable
{
public:
	UITextBox(Font& font, const std::string& txt = "", bool autoWrap = true, float lineSpacing = 0.0f)
		:
		m_font(font),
		m_autoWrap(autoWrap),
		m_lineSpacing(lineSpacing)
	{
		UITextBox::setText(txt);
	}

	virtual ~UITextBox() override
	{}


	void draw(Drawing& draw) override
	{
		float offset = calcLineOffset();
		PointF p = getOrigin();

		for (const std::string& s : m_stringCache)
		{
			m_font.write(draw, s, p);
			p.y += offset;
		}
	}

	virtual void setDim(const PointF& d) override
	{
		UIObject::setDim(d);
		generateText();
	}

	virtual void setText(const std::string& text) override
	{
		ILabelable::setText(text);
		generateText();
	}

	virtual void setAutoWrap(bool autoWrap)
	{
		m_autoWrap = autoWrap;
	}

	bool getAutoWrap() const
	{
		return m_autoWrap;
	}

	virtual void setLineSpacing(bool lineSpacing)
	{
		m_lineSpacing = lineSpacing;
	}

	float getLineSpacing() const
	{
		return m_lineSpacing;
	}

protected:
	virtual float calcLineOffset() const
	{
		float fontHeight = float(m_font.getMaxHeight());
		return fontHeight + fontHeight * m_lineSpacing;
	}

	virtual void generateText()
	{
		m_stringCache.clear();

		PointF dim = getDim();
		const std::string& t = getText();
		size_t length = t.length();

		float lineOffset = calcLineOffset();
		float height = lineOffset;

		bool wrap = false;
		int start = 0;
		float lineWidth = 0.0f;
		std::string s;
		for (int i = 0; i < length; ++i)
		{
			if (height > dim.y)
				break;

			char c = t[i];
			if (c == ' ' || c == '-' || c == '\n' || i + 1 == length)
			{
				int count = i + 1 - start;
				if (c == '\n') // removes \n from string
					--count;
				std::string tmp = t.substr(start, count); // tmp := last word
				lineWidth += m_font.getDim(tmp).x;
				if (lineWidth <= dim.x)
				{
					s.append(tmp);
					start = i + 1;
				}
				else if (m_autoWrap)
				{
					wrap = true;
					--i; // get current word (tmp) again in next loop
				}
				else
				{
					start = i + 1;
				}

				if (c == '\n' || i + 1 == length || wrap)
				{
					m_stringCache.push_back(s);
					s.clear();
					lineWidth = 0.0f;
					height += lineOffset;
					wrap = false;
				}
			}
		}
	}

protected:
	std::vector<std::string> m_stringCache;

private:
	Font& m_font;
	bool m_autoWrap;
	float m_lineSpacing;
};
