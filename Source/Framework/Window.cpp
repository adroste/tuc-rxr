#include "Window.h"

#include "../glew/glew.h"
#include "../System/Log.h"
#include "../Utility/SDL_Exception.h"
#include "../Utility/GL_Exception.h"
#include "../glm/common.hpp"
#include "../glm/gtx/transform.hpp"
#include "OpenGL/Shader/ShaderDisk.h"
#include "OpenGL/Draw.h"
#include "../System/Input.h"

void Window::updateState()
{
}

ShaderDisk sDisk;

void Window::composeFrame()
{
	static float a = 0.0f;
	
	//glRotatef(a * 20.0f, 0.0f, 0.0f, 1.0f);
	//glTranslatef(sin(a), cos(a), 0.0f);
	/*glm::mat4 mat = glm::mat4(1.0f);
	mat = glm::translate(glm::vec3(sin(a), cos(a), 0.0f));
	glLoadMatrixf(&mat[0][0]);*/

	Draw malen;
	malen.rect(RectF::constructFromPoint(Input::getMouse(), 10.0f), Color::Gray());
	
	a += 0.1f;
}

Window::Window()
{
}

Window::~Window()
{
	close();
}

void Window::init(const std::string & title, const PointI & dim)
{
	Log::info("init window");

	m_dim = dim;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_pWnd = SDL_CreateWindow(title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		dim.x, dim.y,
		SDL_WindowFlags::SDL_WINDOW_OPENGL
		| SDL_WindowFlags::SDL_WINDOW_RESIZABLE
	);

	if (!m_pWnd)
		throw SDL_Exception("window creation failed");

	// TODO: load icon

	SDL_SetWindowMinimumSize(m_pWnd, 800, 600);

	// TODO: fullscreen

	// TODO: semaphore
}

void Window::run()
{
	m_pGfx = std::unique_ptr<Graphics>(new Graphics());
	Log::info("initializing openGL");
	m_pGfx->init(m_pWnd, m_dim);
	Log::info("setting swap intervall");
	SDL_GL_SetSwapInterval(1); // vsync

	sDisk.load();
	sDisk.create();

	while (m_isRunning)
	{
		handleEvents();

		updateState();

		m_pGfx->beginFrame();
		composeFrame();
		m_pGfx->endFrame();
		SDL_GL_SwapWindow(m_pWnd);
	}
	m_isRunning = false;
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
			Input::setMouse(PointI(msg.motion.x, msg.motion.y));
			break;
		case SDL_EventType::SDL_MOUSEBUTTONDOWN:
			Input::mouseDown(msg.button.button);
			break;
		case SDL_EventType::SDL_MOUSEBUTTONUP:
			Input::mouseUp(msg.button.button);
			break;
		case SDL_EventType::SDL_MOUSEWHEEL:
			Input::wheel((float)msg.wheel.y);
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
					m_dim = PointI(msg.window.data1, msg.window.data2);
					// TODO lockguard
					m_pGfx->resize(m_dim);
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
	if (m_pGfx)
	{
		m_pGfx->close();
		m_pGfx.reset();
	}

	if (m_pWnd)
	{
		SDL_DestroyWindow(m_pWnd);
		m_pWnd = nullptr;
	}
}

