#include "System\System.h"
#include "Utility\Thread.h"
#include "Utility\Mutex.h"
#include <list>
#include <functional>
#include "System/Log.h"
#include "Framework/Window.h"
#include "System/Input.h"
#include "Utility/Synchronization.h"


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