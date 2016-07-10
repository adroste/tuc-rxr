#pragma once
#include "IPropValue.h"
#include "../../../Utility/Mutex.h"

// thread safe for all containers
template <class T>
class IPropValueSafe : public IPropValue<T>
{
public:
	virtual ~IPropValueSafe()
	{}
	virtual T getValue() const override
	{
		T res;
		LockGuard g(m_muValue);
		res = IPropValue<T>::getValue();
		g.unlock();
		return res;
	}
	virtual void setValue(const T& value) override
	{
		LockGuard g(m_muValue);
		IPropValue<T>::template setValue<T>(value);
		g.unlock();
	}
private:
	Mutex m_muValue;
};
