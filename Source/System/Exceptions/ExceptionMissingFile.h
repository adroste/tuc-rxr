#pragma once
#include "Exception.h"

class ExceptionMissingFile : public Exception
{
public:
	ExceptionMissingFile(const std::string& txt, const std::string& fileName)
		:
		Exception(txt + " - file missing: \"" + fileName + "\"")
	{}
};