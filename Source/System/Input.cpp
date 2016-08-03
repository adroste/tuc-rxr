#include "Input.h"
#include "../Framework/Framework.h"
#include "../Framework/GameState.h"
#include <list>

static PointF m_mousePos;
static std::list<GameState*> m_listener;

void Input::IReceiver::registerMe(GameState* parent)
{
	parent->regReceiver(this);
}

void Input::IReceiver::unregisterMe(GameState* parent)
{
	parent->unregReceiver(this);
}

void Input::init()
{
}

PointF Input::getMouse()
{
	return m_mousePos;
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
		if (l->keyDown(s))
			break;
}

void Input::keyUp(SDL_Scancode s)
{
	for (auto l : m_listener)
		if (l->keyUp(s))
			break;
}

void Input::charDown(char c)
{
	for (auto l : m_listener)
		if (l->charDown(c))
			break;
}

void Input::setMouse(PointI pos)
{
	m_mousePos = Framework::convertClientPoint(pos);

	/*int handled = 0;
	for (auto l : m_listener)
		handled += int(l->mouseMove(m_mousePos, handled != 0));*/
	bool handled = false;
	for (auto l : m_listener)
		handled |= l->mouseMove(m_mousePos, handled);
}

void Input::mouseDown(Uint8 button)
{
	for (auto l : m_listener)
		if (l->mouseDown(Mouse(button), m_mousePos))
			break;
}

void Input::mouseUp(Uint8 button)
{
	for (auto l : m_listener)
		if (l->mouseUp(Mouse(button), m_mousePos))
			break;
}

void Input::wheel(float amount)
{
	for (auto l : m_listener)
		if (l->wheel(amount, m_mousePos))
			break;
}