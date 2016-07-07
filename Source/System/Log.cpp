#include "Log.h"

#include <cstdio>

#include "System.h"
#include "TextFileAppender.h"
#include "../Utility/Date.h"

static const char* LOG_FILENAME = "log.txt";
static const char* LOG_FILENAME_TIME_FORMAT = "Log_%Y-%m-%d_%H-%M-%S.txt";
static const char* TIME_FORMAT = "%Y-%m-%d_%H-%M-%S: ";
static bool LOG_HASERROR = false;
static TextFileAppender logWriter;

void Log::write(const std::string & str)
{
	if (logWriter.isOpen())
		logWriter.writeLine(Date::getTimestring(TIME_FORMAT) + str);
}

void Log::init()
{
	remove(LOG_FILENAME);
	logWriter = TextFileAppender(LOG_FILENAME);
}

void Log::quit()
{
	logWriter.close();
	if (LOG_HASERROR)
	{
		// save log on error
		rename(LOG_FILENAME, Date::getTimestring(LOG_FILENAME_TIME_FORMAT).c_str());
	}
}

void Log::error(const std::exception & e)
{
	LOG_HASERROR = true;
	System::messageBox("ERROR", e.what(), System::BoxIcon::Error);
	write(std::string("ERROR: ") + e.what());
}

void Log::error(const std::exception & e, const std::string & str)
{
	LOG_HASERROR = true;
	std::string tmp = str + " -> " + e.what();
	System::messageBox("ERROR", tmp, System::BoxIcon::Error);
	write(std::string("ERROR: ") + tmp);
}

void Log::warning(const std::string & str)
{
	write(std::string("WARNING: ") + str);
}

void Log::info(const std::string & str)
{
	write(std::string("INFO: ") + str);
}
