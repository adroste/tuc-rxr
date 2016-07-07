#include "Input.h"
#include "../Framework/Framework.h"

static PointF m_mousePos;

void Input::init()
{
}

PointF Input::getMouse()
{

	return m_mousePos;
}

void Input::registerListener(Receiver * listener)
{
}

void Input::unregisterListener(Receiver * listener)
{
}

void Input::keyDown(SDL_Scancode s)
{
}

void Input::keyUp(SDL_Scancode s)
{
}

void Input::charDown(char c)
{
}

void Input::setMouse(PointI pos)
{
	m_mousePos = Framework::convertClientPoint(pos);
}

void Input::mouseDown(Uint8 button)
{
}

void Input::mouseUp(Uint8 button)
{
}

void Input::wheel(float amount)
{
}

Input::Receiver::Receiver()
{
	startListening();
}

Input::Receiver::~Receiver()
{
	stopListening();
}

void Input::Receiver::startListening()
{
	Input::registerListener(this);
}

void Input::Receiver::stopListening()
{
	Input::unregisterListener(this);
}
