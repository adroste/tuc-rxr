#include "System.h"

#if _WIN32
#define _WINSOCKAPI_
#include <windows.h>
#endif

void System::messageBox(const std::string& title, const std::string& message, BoxIcon icon)
{
#if _WIN32
	UINT cicon = 0;
	switch (icon)
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