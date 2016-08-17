#pragma once
#include <functional>

// use CALLBACK( name, args ) to create a callback named: m_onName + setOnNameCallback function
#define CALLBACK(name,...) \
private: std::function<void(__VA_ARGS__)> m_on##name = [](__VA_ARGS__){}; \
public: void setOn##name##Callback(std::function<void(__VA_ARGS__)> c){ \
assert(c != nullptr); m_on##name = c; } private: