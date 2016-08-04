#pragma once
#include "Exception.h"

class ExceptionArgument : public Exception
{
public:
	// TODO ptr to argument
	ExceptionArgument(const std::string& txt, const std::string& argument)
		:
		Exception(txt + " - invalid argument: \"" + argument + "\"")
	{}
};