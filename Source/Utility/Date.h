#pragma once
#include <time.h>
#include <string>

class Date
{

private:
	Date()
	{
		time(&m_rawtime);
		m_timeinfo = localtime(&m_rawtime);
	}
public:
	// format: see http://www.cplusplus.com/reference/ctime/strftime/
	static std::string getTimestring(const std::string& format)
	{
		char buffer[256];
		Date dt;
		strftime(buffer, sizeof(buffer), format.c_str(), dt.m_timeinfo);
		return buffer;
	}

private:
	time_t m_rawtime;
	tm* m_timeinfo;
};