#pragma once

class IClickable
{
public:
	virtual ~IClickable()
	{}
	// removes the clicked status if reset is true
	bool isClicked(bool reset)
	{
		bool t = m_isClicked;
		if(reset)
			m_isClicked = false;
		return t;
	}
protected:
	void setClicked(bool val)
	{
		m_isClicked = val;
	}
private:
	bool m_isClicked = false;
};