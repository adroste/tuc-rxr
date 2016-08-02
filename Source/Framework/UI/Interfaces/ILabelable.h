#pragma once
#include <string>
#include "../../../Utility/Mutex.h"

// thread safe
class ILabelable
{
public:
	virtual ~ILabelable()
	{}
	std::string getText() const
	{
		std::string res;
		LockGuard g(m_muText);
		res = m_text;
		g.unlock();
		return res;
	}
	virtual void setText(const std::string& text)
	{
		LockGuard g(m_muText);
		m_text = text;
		g.unlock();
	}
private:
	std::string m_text;
	Mutex m_muText;
};
