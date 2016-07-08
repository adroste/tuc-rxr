#include <functional>
#include "System/System.h"
#include "System/Log.h"
#include "Framework/Window.h"
#include "System/Input.h"

int main(int argc, char** argv)
{
	Log::init();
	try
	{
		System::init();
		Input::init();

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