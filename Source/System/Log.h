#pragma once
#include <exception>
#include <string>

class Log
{
private:
	Log() {}
	static void write(const std::string& str);

public:
	static void init();
	static void quit();
	static void error(const std::exception& e);
	static void error(const std::exception& e, const std::string& str);
	static void warning(const std::string& str);
	static void info(const std::string& str);
};