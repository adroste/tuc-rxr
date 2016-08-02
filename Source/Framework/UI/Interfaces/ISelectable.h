#pragma once

class ISelectable
{
public:
	virtual ~ISelectable()
	{}

	virtual void select()
	{
		m_isSelected = true;
	}
	virtual void deselect()
	{
		m_isSelected = false;
	}

	virtual bool isSelected() const
	{
		return m_isSelected;
	}
private:
	bool m_isSelected = false;
};