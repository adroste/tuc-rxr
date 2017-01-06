#pragma once
#include <exception>
#include <string>
#include "../../SDL/SDL_net.h"
#include "../../SDL/SDL.h"

class SDL_Exception : public std::exception
{
protected:
	SDL_Exception()
	{}
public:
	SDL_Exception(const std::string& info)
		:
		m_msg(info)
	{
		m_msg += std::string(": ") + std::string(SDL_GetError());
		SDL_ClearError();
	}
#ifdef _WIN32
	virtual const char* what() const override
#else
	virtual const char* what() const noexcept(true) override
#endif
	{
		return m_msg.c_str();
	}
protected:
	std::string m_msg;
};

class SDLNet_Exception : public SDL_Exception
{
public:
	SDLNet_Exception(const std::string& info)
	{
		m_msg = info + std::string(": ") + std::string(SDLNet_GetError());
	}
};
