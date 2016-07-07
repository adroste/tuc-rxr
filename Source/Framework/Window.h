#pragma once
#include <string>
#include <memory>

#include "../SDL/SDL.h"
#include "../Utility/Point.h"
#include "OpenGL/Graphics.h"

class Window
{
public:
	Window();
	virtual ~Window();

	void init(const std::string& title, const PointI& dim);
	void run();
private:
	void handleEvents();
	void close();
	void updateState();
	void composeFrame();
private:
	SDL_Window* m_pWnd = nullptr;
	bool m_isRunning = true;
	PointI m_dim;
	
	std::unique_ptr<Graphics> m_pGfx;
};
