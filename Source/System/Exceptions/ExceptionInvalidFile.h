#pragma once
#include "Exception.h"

class ExceptionInvalidFile : public Exception
{
public:
	ExceptionInvalidFile(const char* txt, const char* fileName)
		:
		Exception(txt),
		m_fileName(fileName)
	{}
	ExceptionInvalidFile(std::string txt, std::string fileName)
		:
		Exception(txt),
		m_fileName(fileName)
	{}

#ifdef _WIN32
	virtual const char* what() const override
#else
	virtual const char* what() const noexcept(true) override
#endif
	{
		return (m_msg + " - file invalid: \"" + m_fileName + "\"").c_str();
	}

protected:
	std::string m_fileName;
};