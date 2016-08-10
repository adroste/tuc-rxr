#pragma once
#include <string>
#include <time.h>

#include "../System/Exceptions/SDL_Exception.h"
#include "Log.h"
#include "../SDL/SDL.h"

class System
{
public:
	enum class BoxIcon
	{
		Warning = SDL_MESSAGEBOX_WARNING,
		Info = SDL_MESSAGEBOX_INFORMATION,
		Error = SDL_MESSAGEBOX_ERROR
	};
public:
	// initialized net an sdl library
	static void init()
	{
		Log::info("System::init SDL");
		Uint32 flags = SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_EVENTS;
		if (SDL_Init(flags) != 0)
			throw SDL_Exception("System::init SDL init failed");

		Log::info("System::init SDLNet");
		if (SDLNet_Init() != 0)
			throw SDLNet_Exception("System::init SDLNet init failed");

		// seed random
		srand(static_cast<unsigned int>(time(nullptr)));
	}
	static void quit()
	{
		SDLNet_Quit();
		SDL_Quit();
	}
	static void enableScreenSaver()
	{
		SDL_EnableScreenSaver();
	}
	static void disableScreenSaver()
	{
		SDL_DisableScreenSaver();
	}
	static bool screenSaverEnabled()
	{
		return SDL_IsScreenSaverEnabled() != SDL_FALSE;
	}
	static void messageBox(const std::string& title, const std::string& message, BoxIcon icon);
	static void sleep(Uint32 ms)
	{
		SDL_Delay(ms);
	}

	// Clipboard
	static bool hasClipboardText()
	{
		return SDL_HasClipboardText() != SDL_FALSE;
	}
	static std::string getClipboardText()
	{
		char* txt = SDL_GetClipboardText();
		std::string res;
		if (txt)
		{
			res = txt;
			SDL_free(txt);
		}
		return res;
	}
	static void setClipboardText(const std::string& t)
	{
		if (!SDL_SetClipboardText(t.c_str()))
		{
			Log::warning("System::setClipboardText could not set clipboard text");
			Log::warning(SDL_Exception("System::setClipboardText").what());
		}
	}

	// screen keyboard
	static bool hasScreenKeyboard()
	{
		return SDL_HasScreenKeyboardSupport() != SDL_FALSE;
	}
	static void startTextInput()
	{
		// TODO touch keyboard?
		SDL_StartTextInput();
	}
	static void stopTextInput()
	{
		SDL_StopTextInput();
	}
	static void hideCursor()
	{
		SDL_ShowCursor(0);
	}
	static void showCursor()
	{
		SDL_ShowCursor(1);
	}
	static void setTrapCursor(bool trap)
	{
		SDL_SetRelativeMouseMode(trap? SDL_TRUE : SDL_FALSE);
	}

	static size_t getThreadID()
	{
		SDL_threadID id = SDL_GetThreadID(nullptr);
		return size_t(id);
	}
private:
	System(){}
};
