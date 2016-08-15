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
		if (!m_isClicked && val)
			clickEvent();

		m_isClicked = val;
	}
	// this function will be triggered if m_isClicked changes to true
	virtual void clickEvent()
	{}
private:
	bool m_isClicked = false;
};