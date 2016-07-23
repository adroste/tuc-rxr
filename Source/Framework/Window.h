#pragma once
#include <string>
#include <memory>
#include <list>

#include "../SDL/SDL.h"
#include "../Utility/Point.h"
#include "OpenGL/Graphics.h"
#include "GameState.h"
#include "../Utility/Thread.h"
#include "../Utility/Condition.h"

class Window : public Thread
{
public:
	Window();
	virtual ~Window();

	void init(const std::string& title, const PointI& dim);
	void run();
private:
	void handleEvents();
	void close();
	void updateState(float dt);
	void composeFrame(float dt);
protected:
	virtual int threadProc() override;
private:
	SDL_Window* m_pWnd = nullptr;
	bool m_isRunning = true;
	// true if graphics thread init failed or succeded
	bool m_gfxInitThreadDone = false;
	// true if grafics thread succeded
	bool m_gfxIsInit = false;
	PointI m_dim;
	Condition m_condGfx;
	Mutex m_muGfx;
	
	std::unique_ptr<Graphics> m_pGfx;
	std::list<std::unique_ptr<GameState>> m_states;
};
