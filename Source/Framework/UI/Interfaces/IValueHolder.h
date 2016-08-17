#pragma once
#include "../Callback.h"

// only thread safe for thread safe type (int, float...)
template <class T>
class IValueHolder
{
	CALLBACK(Value, IValueHolder*);
public:
	IValueHolder()
	{
	}

	IValueHolder(T val)
		:
		m_value(val)
	{
	}

	virtual ~IValueHolder()
	{
	}

	virtual T getValue() const
	{
		return m_value;
	}

	virtual void setValue(const T& value)
	{
		m_value = value;
		m_onValue(this);
	}

private:
	T m_value;
};
