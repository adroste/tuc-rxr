#pragma once
#include "UINumUpDown.h"

class UINumUpDownFloat : public UINumUpDown<float>
{
public:
	UINumUpDownFloat(Font& font, float start, float min, float max, float step)
		: UINumUpDown<float>(font, start, min, max, step)
	{
		setRegex("[\\d+-]\\d*\\.?\\d*");
		UINumUpDown<float>::setValue(start);
	}

protected:
	std::string numToSting(float n) const override
	{
		auto s = std::to_string(n);
		// cut unnecessary 0's
		if(s.find('.') != std::string::npos)
		{
			// something like 1.002000
			s.erase(s.find_last_not_of('0') + 1, std::string::npos);
			if (s.back() == '.')
				s.pop_back();
		}

		return s;
	}

	float stringToNum(const std::string& s) const override
	{
		float res = 0.0f;
		try
		{
			res = std::stof(s);
		}
		catch (const std::exception&)
		{
		}
		return res;
	}
};
