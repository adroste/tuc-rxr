#pragma once
#include "../Callback.h"

class IClickable
{
	CALLBACK(Click,IClickable*);
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
		/*if (!m_isClicked && val)
			m_onClick(this);*/
		if (val)
			m_onClick(this);

		m_isClicked = val;
	}
private:
	
	bool m_isClicked = false;
};