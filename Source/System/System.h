#pragma once
#include <string>
#include <time.h>

#include "../System/Exceptions/SDL_Exception.h"
#include "Log.h"
#include "../SDL/SDL.h"

#if _WIN32
#define _WINSOCKAPI_
#include <windows.h>
#endif

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
	static void messageBox(const std::string& title, const std::string& message, BoxIcon icon)
	{
#if _WIN32
		UINT cicon = 0;
		switch(icon)
		{
		case BoxIcon::Warning:
			cicon = MB_ICONWARNING;
			break;
		case BoxIcon::Info:
			cicon = MB_ICONINFORMATION;
			break;
		case BoxIcon::Error:
			cicon = MB_ICONERROR;
			break;
		default: break;
		}
		MessageBoxA(nullptr, message.c_str(), title.c_str(), MB_OK | cicon);
#else
		SDL_MessageBoxData dat;
		dat.flags = static_cast<Uint32>(icon);
		dat.window = nullptr;
		dat.title = title.c_str();
		dat.message = message.c_str();

		// buttons
		SDL_MessageBoxButtonData btn;
		btn.buttonid = 0;
		btn.flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
		btn.text = "ok";

		dat.numbuttons = 1;
		dat.buttons = &btn;

		dat.colorScheme = nullptr;

		int pressed;
		SDL_ShowMessageBox(&dat, &pressed);
#endif
	}
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

	static size_t getThreadID()
	{
		SDL_threadID id = SDL_GetThreadID(nullptr);
		return size_t(id);
	}
private:
	System(){}
};
