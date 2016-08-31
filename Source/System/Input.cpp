#include "Input.h"
#include <list>
#include "../Framework/Framework.h"
#include "../Framework/GameState.h"
#include "../Framework/Window.h"

static PointF m_mousePos;
static PointF m_mouseDiff;
static std::list<GameState*> m_listener;
static Window* m_pWnd = nullptr;
static bool m_isMouseTrapped = false;

void Input::init(Window* pWnd)
{
	assert(pWnd);
	m_pWnd = pWnd;
}

PointF Input::getMouse()
{
	return m_mousePos;
}

void Input::setCamMouse(const PointF& p)
{
	PointI cpos = Framework::convertCamPoint(p);
	m_pWnd->setMouse(cpos);
}

void Input::trapMouse()
{
	System::setTrapCursor(true);
	m_isMouseTrapped = true;
}

void Input::freeMouse()
{
	
	m_isMouseTrapped = false;
	System::setTrapCursor(false);
	setCamMouse(m_mousePos);
}

bool Input::isMouseTrapped()
{
	return m_isMouseTrapped;
}

void Input::registerState(GameState* pState)
{
	m_listener.push_front(pState);
}

void Input::unregisterState(GameState* pState)
{
	m_listener.remove_if([pState](GameState* src)
	{
		return pState == src;
	});
}

void Input::keyDown(SDL_Scancode s)
{
	for (auto l : m_listener)
		if (l->isEnabled() && l->keyDown(s))
			break;
}

void Input::keyUp(SDL_Scancode s)
{
	for (auto l : m_listener)
		if (l->isEnabled() && l->keyUp(s))
			break;
}

void Input::charDown(char c)
{
	for (auto l : m_listener)
		if (l->isEnabled() && l->charDown(c))
			break;
}

void Input::setMouse(PointI pos, PointI diff)
{
	if (!m_isMouseTrapped)	
		m_mousePos = Framework::convertClientPoint(pos);

	m_mouseDiff = Framework::convertClientPoint(diff);

	bool handled = false;
	for (auto l : m_listener)
	{
		if (!l->isEnabled())
			// just to update mouse position
			l->mouseMove(m_mousePos, m_mouseDiff, true);
		else
			handled |= l->mouseMove(m_mousePos, m_mouseDiff, handled);
	}
}


void Input::mouseDown(Uint8 button)
{
	for (auto l : m_listener)
		if (l->isEnabled() && l->mouseDown(m_mousePos, Mouse(button)))
			break;
}

void Input::mouseUp(Uint8 button)
{
	for (auto l : m_listener)
		if (l->isEnabled() && l->mouseUp(m_mousePos, Mouse(button)))
			break;
}

void Input::wheel(float amount)
{
	for (auto l : m_listener)
		if (l->isEnabled() && l->wheel(m_mousePos, amount))
			break;
}