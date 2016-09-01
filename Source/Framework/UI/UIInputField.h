#pragma once
#include <regex>
#include "UIObject.h"
#include "../../System/Input.h"
#include "Interfaces/ILabelable.h"
#include "../../System/System.h"
#include "Interfaces/ISelectable.h"
#include "Callback.h"

// url regex
//  (?<=\s|^)["'([{]*(https?:\/\/)?(\w+\.)?(\w+\.)(\w{2,})((?:\/[^\s?]+)+)?\/?(\?(?:[^\s?]+)?)?['")]}]*(?=[.,:;?!]*(?:\s|$))

class UIInputField : public UIObject, public ILabelable, public ISelectable
{
	CALLBACK(Finish, UIInputField*);
	CALLBACKPROTECTED(FinishProtected, UIInputField*);
public:
	UIInputField(Font& font, size_t maxLen)
		:
		m_font(font),
		m_maxLen(maxLen)
	{
		m_regex = "[ -~]*";
		// TODO set metrics?
	}
	virtual ~UIInputField() override
	{}
	
	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		m_font.setColor(Color::White());
		draw.rect(getRectPad(), Color::DarkGray());
		m_font.write(draw, getDisplayText(), getOriginPad());
	}
	void setRegex(const std::string& r)
	{
		m_regex = r;
	}

	virtual void deselect() override
	{
		if(isSelected())
		{
			ISelectable::deselect();
			m_onFinishProtected(this);
			m_onFinish(this);			
		}
	}

	virtual bool keyDown(SDL_Scancode s) override
	{
		if(isSelected())
		{
			switch (s)
			{
			case SDL_SCANCODE_LEFT:
				if (m_cursorPos > 0)
					m_cursorPos--;
				break;
			case SDL_SCANCODE_RIGHT:
				if (m_cursorPos < getText().length())
					m_cursorPos++;
				break;
			case SDL_SCANCODE_DELETE:
				if (m_cursorPos < getText().length())
				{
					auto str = getText();
					LockGuard g(m_muCursor);
					str.erase(m_cursorPos,1);
					ILabelable::setText(str);
				}
				break;
			case SDL_SCANCODE_BACKSPACE:
				if(m_cursorPos > 0)
				{
					auto str = getText();
					LockGuard g(m_muCursor);
					str.erase(m_cursorPos - 1,1);
					m_cursorPos--;
					ILabelable::setText(str);
				}
				break;
			case SDL_SCANCODE_C:
				// copy?
				if(m_ctrlDown)
				{
					System::setClipboardText(getText());
				}
				break;
			case SDL_SCANCODE_V:
				// pasta?
				if(m_ctrlDown)
				{
					if(System::hasClipboardText())
					{
						for (const auto& c : System::getClipboardText())
							addChar(c);
					}
				}
				break;
			case SDL_SCANCODE_LCTRL:
				m_ctrlDown = true;
				break;
			}
			return true;
		}
		return false;
	}
	virtual bool keyUp(SDL_Scancode s) override
	{
		if (isSelected())
		{
			switch (s)
			{
			case SDL_SCANCODE_RETURN:
			case SDL_SCANCODE_ESCAPE:
				deselect();
				System::stopTextInput();
				return true;
			}
		}

		if(s == SDL_SCANCODE_LCTRL)
			m_ctrlDown = false;
					
		return false;
	}
	virtual bool charDown(char c) override
	{
		if (isSelected())
		{
			addChar(c);
			return true;
		}
		return false;
	}
	virtual bool mouseMove(const PointF& mpos, const PointF& mdiff, bool handled) override
	{
		if(getRect().isPointInside(mpos))
		{
			m_isHover = true;
		}
		else
		{
			m_isHover = false;
		}
		return false;
	}
	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		if(m_isHover && button == Input::Mouse::Left)
		{
			select();
			System::startTextInput();
			return true;
		}
		else
		{
			deselect();
			return false;
		}
	}
	void clear()
	{
		setText("");
		m_cursorPos = 0;
	}


	virtual void setText(const std::string& text) override
	{
		ILabelable::setText(text);
		m_cursorPos = text.length();
	}
protected:
	void addChar(char c)
	{
		if(getText().length() < m_maxLen)
		{
			std::string ins = std::string(1, c);

			std::string s = getText();
			// insert char at cursor pos
			if (m_cursorPos == s.length())
				s.push_back(c);
			else
				s.insert(m_cursorPos, ins);

			if(std::regex_match(s, std::regex(m_regex)))
			{
				LockGuard g(m_muCursor);
				ILabelable::setText(s);
				m_cursorPos++;
				g.unlock();
			}
		}
	}
	std::string getDisplayText()
	{
		static int timer = 0;

		float wi = getDimPad().x;
		if(isSelected())
		{
			// draw with cool | animation
			std::string toAdd = std::string(1, ++timer % 30 <= 15 ? '|' : 0x1f);

			LockGuard g(m_muCursor);
			std::string res = getText();
			if (res.length() == m_cursorPos)
				res += toAdd;
			else
				res.insert(m_cursorPos,toAdd);
			g.unlock();

			if(m_font.getDim(res).x > wi)
			{
				while (m_font.getDim(res).x > wi && m_cursorPos + 4 < res.length())
					res.pop_back();
			}

			while (m_font.getDim(res).x > wi)
				res = res.substr(1, res.length() - 1);
			return res;
		}
		else
		{
			// draw plain text
			std::string res = getText();
			while (m_font.getDim(res).x > wi)
				res.pop_back();
			return res;
		}
	}

private:
	Font& m_font;
	const size_t m_maxLen;

	bool m_isHover = false;
	bool m_ctrlDown = false; 
	std::string m_regex;

	// TODO add mutex
	Mutex m_muCursor;
	size_t m_cursorPos = 0;
};
