#pragma once
#include "Exception.h"

class ExceptionInvalidOperation : public Exception
{
public:
	ExceptionInvalidOperation(const std::string& txt, const std::string& stateDescription)
		:
		Exception(txt + " - invalid operation: \"" + stateDescription + "\"")
	{}
};
