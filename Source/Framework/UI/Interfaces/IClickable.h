#pragma once

class IClickable
{
	std::function<void(IClickable*)> m_onClick = [](IClickable*) {};
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
	void setOnClickCallback(decltype(m_onClick) c)
	{
		assert(c != nullptr);
		m_onClick = c;
	}
protected:
	void setClicked(bool val)
	{
		if (!m_isClicked && val)
			m_onClick(this);

		m_isClicked = val;
	}
private:
	
	bool m_isClicked = false;
};