#pragma once
#include <exception>
#include <string>

class Exception : public std::exception
{
public:
    Exception(const char* txt)
    {
        m_msg = txt;
    }
    Exception(const std::string& s)
    {
        m_msg = s;
    }
#ifdef _WIN32
	virtual const char* what() const override
#else
	virtual const char* what() const noexcept(true) override
#endif
	{
		return m_msg.c_str();
	}
protected:
    std::string m_msg;
};
