#pragma once
#include "UIObject.h"
#include "UIInputBox.h"
#include "Interfaces/IValueHolder.h"

template <class T>
class UINumUpDown : public UIInputBox, public IValueHolder<T>
{
public:
	UINumUpDown(Font& font, T start, T min, T max, T step)
		: 
		UIInputBox(font, 256),
		IValueHolder(start),
		m_step(step),
		m_min(min),
		m_max(max)
	{
		//setRegex("0-9");
		setOnFinishCallback([this](UIInputField* inp)
		{
			// convert input string to Value Type T
			this->setValue(stringToNum(inp->getText()));
		});
	}
	virtual ~UINumUpDown()
	{}

	void setValue(const T& value) override
	{
		// set clamped value
		IValueHolder<T>::setValue(tool::clamp(value, m_min, m_max));
		// set text according to new value
		setText(numToSting(this->getValue()));
	}

	bool keyDown(SDL_Scancode s) override
	{
		if(isSelected())
		{
			switch (s)
			{
			case SDL_SCANCODE_UP:
				setValue(addValue(this->getValue(),m_step));
				return true;
			case SDL_SCANCODE_DOWN:
				setValue(addValue(this->getValue(), negateValue(m_step)));
				return true;
			}
		}
		
		return UIInputBox::keyDown(s);
	}
protected:
	virtual std::string numToSting(T n) const = 0;
	virtual T stringToNum(const std::string& s) const = 0;
	virtual T addValue(T left, T right) = 0;
	virtual T negateValue(T val) = 0;
private:
	const T m_step,
		m_min, m_max;
};
