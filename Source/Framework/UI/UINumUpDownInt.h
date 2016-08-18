#pragma once
#include "UINumUpDown.h"

class UINumUpDownInt : public UINumUpDown<int>
{
public:
	UINumUpDownInt(Font& font, int start, int min, int max, int step)
		: UINumUpDown<int>(font, start, min, max, step)
	{
		setRegex("[\\d+-]\\d*");
		UINumUpDown<int>::setValue(start);
	}

protected:
	std::string numToSting(int n) const override
	{
		return std::to_string(n);
	}

	int stringToNum(const std::string& s) const override
	{
		int res = 0;
		try
		{
			res = std::stoi(s);
		}
		catch(const std::exception&)
		{ }
		return res;
	}
	virtual int negateValue(int val) override
	{
		return -val;
	}
	virtual int addValue(int left, int right) override
	{
		return left + right;
	}
};
