#pragma once
#include "../SDL/SDL.h"
#include "../Utility/Point.h"
#include "../glm/glm.hpp"
#include <list>
#include "Exceptions/ExceptionInvalidOperation.h"
#include "Exceptions/ExceptionArgument.h"

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

	class IBroadcaster;

	class IReceiver
	{
		friend class Input;
	public:
		IReceiver(int zIndex = 0)
			:
			m_zIndex(zIndex),
			m_transform(1.0f)
		{}
		virtual ~IReceiver()
		{}

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

		virtual void registerMe(IBroadcaster* broadcaster)
		{
			m_broadcaster = broadcaster;
			if (!m_broadcaster)
				throw ExceptionArgument("Input::IReceiver::registerMe", "nullptr");
			m_broadcaster->regReceiver(this);
		}
		virtual void unregisterMe()
		{
			if (!m_broadcaster)
				throw ExceptionInvalidOperation("Input::IReceiver::unregisterMe receiver not registered", "m_broadcaster = nullptr");
			m_broadcaster->unregReceiver(this);
		}
	private:
		bool m_enabled = true;
		int m_zIndex;
		glm::mat4 m_transform;
		IBroadcaster* m_broadcaster = nullptr;
	};

	class IBroadcaster
	{
	public:
		IBroadcaster()
		{}
		virtual ~IBroadcaster()
		{}

		void sortReceivers()
		{
			m_receivers.sort([](Input::IReceiver* lr, Input::IReceiver* rr)
			{
				return lr->getZIndex() > rr->getZIndex();
			});
		}

		void regReceiver(Input::IReceiver* rec)
		{
			for (auto r : m_receivers)
				if (r == rec)
					throw ExceptionInvalidOperation("GameState::regReceiver try to add receiver twice", "receiver already in m_receivers");
			m_receivers.push_front(rec);
			sortReceivers();
		}

		void unregReceiver(Input::IReceiver* rec)
		{
			m_receivers.remove_if([rec](Input::IReceiver* curRec)
			{
				return rec == curRec;
			});
		}

		virtual bool sendKeyDown(SDL_Scancode s)
		{
			return handleKey(&Input::IReceiver::keyDown, s);
		}
		virtual bool sendKeyUp(SDL_Scancode s)
		{
			return handleKey(&Input::IReceiver::keyUp, s);
		}
		virtual bool sendCharDown(char c)
		{
			return handleKey(&Input::IReceiver::charDown, c);
		}
		virtual bool sendMouseMove(const PointF& mpos, bool handled)
		{
			int curZ = -1;
			bool prevHandled = handled;

			for (auto r : m_receivers)
			{
				PointF p = r->transformInpPoint(mpos);

				if (!r->isEnabled())
				{
					// just to update mouse pos
					r->mouseMove(p, true);
					continue;
				}

				if (curZ != r->getZIndex())
				{
					prevHandled = handled;
				}

				bool nowHandled = r->mouseMove(p, prevHandled);
				handled = nowHandled || handled;
			}
			return handled;
		}
		virtual bool sendMouseDown(const PointF& mpos, Input::Mouse button)
		{
			return handleKey(&Input::IReceiver::mouseDown, mpos, button);
		}
		virtual bool sendMouseUp(const PointF& mpos, Input::Mouse button)
		{
			return handleKey(&Input::IReceiver::mouseUp, mpos, button);
		}
		virtual bool sendWheel(const PointF& mpos, float amount)
		{
			return handleKey(&Input::IReceiver::wheel, mpos, amount);
		}

	private:
		template <typename memFunc, typename... ArgT>
		bool handleKey(memFunc pFunc, const PointF& mpos, ArgT... args)
		{
			int curZ = -1;
			bool handled = false;
			for (auto r : m_receivers)
			{
				if (!r->isEnabled())
					continue;
				if (handled && curZ != r->getZIndex())
					break;
				curZ = r->getZIndex();
				if (((*r).*pFunc)(r->transformInpPoint(mpos), args...))
					handled = true;
			}
			return handled;
		}
		template <typename memFunc, typename... ArgT>
		bool handleKey(memFunc pFunc, ArgT... args)
		{
			int curZ = -1;
			bool handled = false;
			for (auto r : m_receivers)
			{
				if (!r->isEnabled())
					continue;
				if (handled && curZ != r->getZIndex())
					break;
				curZ = r->getZIndex();
				if (((*r).*pFunc)(args...))
					handled = true;
			}
			return handled;
		}

	protected:
		std::list<Input::IReceiver*> m_receivers;
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
