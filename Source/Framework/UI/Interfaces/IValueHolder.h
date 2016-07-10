#pragma once

// only thread safe for thread safe type (int, float...)
template <class T>
class IValueHolder
{
public:
	virtual ~IValueHolder()
	{}
	virtual T getValue() const
	{
		return m_value;
	}
	virtual void setValue(const T& value)
	{
		m_value = value;
	}
private:
	T m_value;
};