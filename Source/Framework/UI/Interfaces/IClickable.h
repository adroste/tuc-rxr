#pragma once

class IClickable
{
public:
	virtual ~IClickable()
	{}
	// removes the clicked status after querying
	bool isClicked()
	{
		bool t = m_isClicked;
		m_isClicked = false;
		return t;
	}
protected:
	bool m_isClicked = false;
};