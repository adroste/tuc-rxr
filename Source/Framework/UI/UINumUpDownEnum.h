#pragma once
#include "UINumUpDown.h"

template <class T>
class UINumUpDownEnum : public UINumUpDown<T>
{
	static_assert(std::is_enum<T>::value,"UINumUpDownEnum Type must be enum");
protected:
	UINumUpDownEnum(Font& font, T start, T min, T max)
		: UINumUpDown<T>(font, start, min, max, T(1))
	{
	}

	virtual T addValue(T left, T right) override
	{
		return T(long(left) + long(right));
	}
	virtual T negateValue(T val) override
	{
		return T(-long(val));
	}
};