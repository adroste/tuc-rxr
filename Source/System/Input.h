#pragma once
#include "../SDL/SDL.h"
#include "../Utility/Point.h"
#include "../glm/glm.hpp"

class Input
{
	friend class Window;

public:
	enum class Mouse
	{
		Left = SDL_BUTTON_LEFT,
		Middle = SDL_BUTTON_MIDDLE,
		Right = SDL_BUTTON_RIGHT,
		X1 = SDL_BUTTON_X1,
		X2 = SDL_BUTTON_X2
	};

	class IReceiver
	{
		friend class Input;
	public:
		IReceiver(int zIndex = 0)
			:
			m_zIndex(zIndex),
			m_transform(1.0f)
		{}
		virtual ~IReceiver(){}

		virtual bool keyDown(SDL_Scancode s)
		{
			return false;
		}
		virtual bool keyUp(SDL_Scancode s) 
		{
			return false;
		}
		virtual bool charDown(char c)
		{
			return false;
		}
		virtual bool mouseMove(const PointF& mpos, bool handled)
		{
			return false;
		}
		virtual bool mouseDown(const PointF& mpos, Mouse button)
		{
			return false;
		} 
		virtual bool mouseUp(const PointF& mpos, Mouse button)
		{
			return false;
		}
		virtual bool wheel(const PointF& mpos, float amount)
		{
			return false;
		}

		virtual void enable()
		{
			m_enabled = true;
		}
		virtual void disable()
		{
			m_enabled = false;
		}
		virtual bool isEnabled() const
		{
			return m_enabled;
		}

		int getZIndex() const
		{
			return m_zIndex;
		}

		virtual void setZIndex(int z)
		{
			m_zIndex = z;
		}

		glm::mat4 getInpTransform() const
		{
			return m_transform;
		}

		virtual void setInpTransform(glm::mat4 transform)
		{
			m_transform = transform;
		}

		virtual PointF transformInpPoint(const PointF& p)
		{
			glm::vec4 pos = getInpTransform() * glm::vec4(p.x, p.y, 0.0f, 1.0f);
			return { pos.x, pos.y };
		}

		virtual void registerMe(class GameState* parent);
		virtual void unregisterMe(class GameState* parent);
	private:
		bool m_enabled = true;
		int m_zIndex;
		glm::mat4 m_transform;
	};

public:
	static void init();
	static PointF getMouse();

	static void registerState(class GameState* pState);
	static void unregisterState(class GameState* pState);
private:
	static void keyDown(SDL_Scancode s);
	static void keyUp(SDL_Scancode s);
	static void charDown(char c);

	static void setMouse(PointI pos);
	static void mouseDown(Uint8 button);
	static void mouseUp(Uint8 button);

	static void wheel(float amount);
};
