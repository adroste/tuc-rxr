#pragma once
#include "Exception.h"

class ExceptionInvalidFile : public Exception
{
public:
	ExceptionInvalidFile(const std::string& txt, const std::string& fileName)
		:
		Exception(txt + " - file invalid: \"" + fileName + "\"")
	{}
};