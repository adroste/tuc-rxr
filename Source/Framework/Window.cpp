#include "Window.h"

#include "../System/Log.h"
#include "../System/Exceptions/SDL_Exception.h"
#include "../glm/gtx/transform.hpp"
#include "OpenGL/Shader/ShaderDisk.h"
#include "../System/Input.h"
#include "../Game/Client/States/StateMenu.h"
#include "../Game/Client/States/StateGame.h"
#include "../Utility/Timer.h"
#include "../System/System.h"
#include <stack>

static const int MAX_UPDATE_DELAY = 15;

Window::Window()
{
}

Window::~Window()
{
	close();
}

void Window::init(const std::string & title, const PointI & dim)
{
	Log::info("Window::init");

	m_dim = dim;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_pWnd = SDL_CreateWindow(title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		dim.x, dim.y,
		SDL_WindowFlags::SDL_WINDOW_OPENGL
		| SDL_WindowFlags::SDL_WINDOW_RESIZABLE
		| SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI
	);

	if (!m_pWnd)
		throw SDL_Exception("Window::init window creation failed");

	Input::init(this);

	// TODO: load icon

	SDL_SetWindowMinimumSize(m_pWnd, 800, 600);

	// TODO: fullscreen

	Thread::begin();
	LockGuard guard(m_muGfx);
	m_condGfx.wait(guard, [this]() {return m_gfxInitThreadDone;});
	guard.unlock();

	if(hasThreadError()) // gfx init failed?
		throw Exception("Window::init " + getThreadError());
	
	assert(m_gfxIsInit);
}

void Window::run()
{
	Log::info("Window::run starting game");
	{
		LockGuard g(m_muGfx);
		m_states.push_front(owner_ptr<GameState>(new StateMenu()));
		Input::registerListener(m_states.front().getRef());
		g.unlock();
	}

	Timer t;
	t.startWatch();

	PointI oldStdDraw = Framework::getStdDraw();

	while (m_isRunning)
	{
		assert(m_states.size());

		handleEvents();

		// handle stdDraw change
		if (oldStdDraw != Framework::getStdDraw())
		{
			oldStdDraw = Framework::getStdDraw();
			LockGuard g(m_muGfx);
			m_pGfx->resize(m_dim);
			g.unlock();
			for (auto& s : m_states)
				s->onResize();
		}

		float dt = t.lapSecond();
		updateState(dt);

		Sound::update(dt);

		// state transition
		if (m_states.front()->isFinished())
		{
			auto pNext = m_states.front()->popNextState();
			LockGuard g(m_muGfx);

			auto tranState = m_states.front()->getTransitionState();
			if (tranState == GameState::TransitionState::Discard 
				|| tranState == GameState::TransitionState::DiscardWithPrevious)
			{
				do
				{
					tranState = m_states.front()->getTransitionState();
					if (tranState != GameState::TransitionState::ForcePreserve)
					{
						// remove object from m_states + delete it (unique_ptr)
						m_states.pop_front();
					}
					assert(m_states.size());
				} while (tranState == GameState::TransitionState::DiscardWithPrevious
					|| tranState == GameState::TransitionState::PreserveOrDiscardWithPrevious);
			}

			if (pNext)
			{
				Input::registerListener(pNext.getRef());
				m_states.push_front(std::move(pNext));
			}
			g.unlock();
		}

		int timeToSleep = MAX_UPDATE_DELAY - int(t.getTimeMilli());
		if (timeToSleep > 0)
			System::sleep(timeToSleep);

		// check for thread error
		if(hasThreadError())
		{
			// TODO delete states
			throw Exception("Window::threadProc " + getThreadError());
		}
	}
	m_isRunning = false;
}

void Window::setMouse(PointI p)
{
	SDL_WarpMouseInWindow(m_pWnd, p.x, p.y);
}

void Window::handleEvents()
{
	SDL_Event msg;
	while (SDL_PollEvent(&msg))
	{
		switch (msg.type)
		{
		case SDL_EventType::SDL_QUIT:
			m_isRunning = false;
			break;

			/////////////////////////////////////
			/////////////// Mouse ///////////////
			/////////////////////////////////////

		case SDL_EventType::SDL_MOUSEMOTION:
			Input::setMouse(PointI(msg.motion.x, msg.motion.y), PointI(msg.motion.xrel, msg.motion.yrel));
			break;
		case SDL_EventType::SDL_MOUSEBUTTONDOWN:
			Input::mouseDown(msg.button.button);
			break;
		case SDL_EventType::SDL_MOUSEBUTTONUP:
			Input::mouseUp(msg.button.button);
			break;
		case SDL_EventType::SDL_MOUSEWHEEL:
			Input::wheel(float(msg.wheel.y));
			break;

			/////////////////////////////////////
			/////////////// Keyboard ////////////
			/////////////////////////////////////

		case SDL_EventType::SDL_KEYUP:
			Input::keyUp(msg.key.keysym.scancode);
			break;
		case SDL_EventType::SDL_KEYDOWN:
			Input::keyDown(msg.key.keysym.scancode);
			break;

		case SDL_EventType::SDL_TEXTINPUT:
		{
			const char* c = msg.text.text;
			int len = 0;
			while (*c != '\0' && len < 31)
			{
				Input::charDown(*c);
				c++;
				len++;
			}
		}
		break;
		case SDL_EventType::SDL_WINDOWEVENT:
			switch (msg.window.event)
			{
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			case SDL_WINDOWEVENT_RESIZED:
				if (m_pGfx)
				{
					PointI prevDim = m_dim;
					m_dim = PointI(msg.window.data1, msg.window.data2);

					if(m_dim != prevDim)
					{
						LockGuard g(m_muGfx);
						m_pGfx->resize(m_dim);
						g.unlock();

						for (auto& s : m_states)
							s->onResize();
					}
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}


	}
}

void Window::close()
{
	m_isRunning = false;
	Thread::join();

	if (m_pGfx)
	{
		m_pGfx.reset();
	}

	if (m_pWnd)
	{
		SDL_DestroyWindow(m_pWnd);
		m_pWnd = nullptr;
	}
}

void Window::updateState(float dt)
{
	for (auto& s : m_states)
		if (!s->update(dt))
			break;
}

void Window::composeFrame(float dt)
{
	if(m_states.size())
	{
		std::stack<GameState*> s;

		for(auto& state : m_states)
		{
			s.push(state.get());
			if (!state->drawPreviousState())
				break;
		}

		while(!s.empty())
		{
			s.top()->composeFrame(m_pGfx->getDraw(), dt);
			s.pop();
		}
	}
}

int Window::threadProc()
{
	// init gfx
	try
	{
		m_pGfx = std::unique_ptr<Graphics>(new Graphics());
		Log::info("Window::threadProc initializing openGL");
		m_pGfx->init(m_pWnd, m_dim);
		Log::info("Window::threadProc setting swap intervall");
		SDL_GL_SetSwapInterval(1); // vsync
	}
	catch(const std::exception& e)
	{
		// init failed
		m_gfxInitThreadDone = true;
		setThreadError(e.what());
		// wake up main thread
		m_condGfx.notifyAll();
		return -1;
	}
	m_gfxIsInit = true;
	m_gfxInitThreadDone = true;
	// wake up main thread
	m_condGfx.notifyAll();

	try
	{
		Timer t;
		t.startWatch();
		// start main loop
		while (m_isRunning)
		{
			LockGuard g(m_muGfx);
			if (m_states.size())
			{
				// states are available to draw
				m_pGfx->beginFrame();
				float dt = t.lapSecond();
				composeFrame(dt);
				// unlock because we are finished with drawing state relevant objects
				g.unlock();
				m_pGfx->endFrame();
				SDL_GL_SwapWindow(m_pWnd);
			}
			else
			{
				g.unlock();
				System::sleep(1);
			}
		}

		m_pGfx->close();
	}
	catch(const std::exception& e)
	{
		setThreadError(e.what());
	}

	return 0;
}
