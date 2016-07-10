#pragma once
#include "IPropValue.h"
#include "../../../Utility/Mutex.h"

// thread safe for all containers
template <class T>
class IValueHolderSafe : public IValueHolder<T>
{
public:
	virtual ~IValueHolderSafe()
	{}
	virtual T getValue() const override
	{
		T res;
		LockGuard g(m_muValue);
		res = IValueHolder<T>::getValue();
		g.unlock();
		return res;
	}
	virtual void setValue(const T& value) override
	{
		LockGuard g(m_muValue);
		IValueHolder<T>::template setValue<T>(value);
		g.unlock();
	}
private:
	Mutex m_muValue;
};
