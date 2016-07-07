#pragma once
#include "../SDL/SDL.h"
#include "../Utility/Point.h"

class Input
{
	friend class Window;

public:
	class Receiver
	{
		friend class Input;
	public:
		Receiver();
		virtual ~Receiver();

		void startListening();
		void stopListening();
	};

public:
	static void init();
	static PointF getMouse();

private:
	static void registerListener(Receiver* listener);
	static void unregisterListener(Receiver* listener);

	static void keyDown(SDL_Scancode s);
	static void keyUp(SDL_Scancode s);
	static void charDown(char c);

	static void setMouse(PointI pos);
	static void mouseDown(Uint8 button);
	static void mouseUp(Uint8 button);

	static void wheel(float amount);
};