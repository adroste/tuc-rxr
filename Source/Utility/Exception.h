#pragma once
#include <string>
#include <exception>

#ifdef _MSC_VER
// visual c++
#define FUNC_NAME std::string(__FUNCSIG__)
#else
// g++
#define FUNC_NAME std::string(__PRETTY_FUNCTION__)
#endif

// c++ standart

#define FUNC_EXACT_POS std::string(FUNC_NAME + std::string(" @line: ") + std::to_string(__LINE__))

#define STD_EXCEPTION std::exception(FUNC_EXACT_POS)