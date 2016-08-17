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
		IValueHolder(),
		m_step(step),
		m_min(min),
		m_max(max)
	{}
	virtual ~UINumUpDown()
	{}
	
protected:
	virtual std::string numToSting(T n) const = 0;
	virtual T stringToNum(const std::string& s) const = 0;

private:
	const T m_step,
		m_min, m_max;
};
