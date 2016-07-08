#include <functional>
#include "System/System.h"
#include "System/Log.h"
#include "Framework/Window.h"
#include "System/Input.h"

#include "SDL/SDL_mutex.h"

void test()
{
	SDL_mutex* mu = SDL_CreateMutex();
	SDL_LockMutex(mu);
	SDL_LockMutex(mu);
	SDL_UnlockMutex(mu);
	SDL_UnlockMutex(mu);
	SDL_DestroyMutex(mu);
}

int main(int argc, char** argv)
{
	Log::init();
	try
	{
		System::init();
		Input::init();

		test();

		Window wnd;
		wnd.init("MainWindow", { 1280, 720 });
		wnd.run();
	}
	catch (const std::exception& e)
	{
		Log::error(e);
	}
	System::quit();
	Log::quit();

	return 0;
}