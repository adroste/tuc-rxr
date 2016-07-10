#pragma once
#include "SDL_Exception.h"
#include "../SDL/SDL_mixer.h"

class MIX_Exception : public SDL_Exception
{
public:
	MIX_Exception(const std::string& info)
	{
		m_msg = info + std::string(": ") + std::string(Mix_GetError());
	}
};